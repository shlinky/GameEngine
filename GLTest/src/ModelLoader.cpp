#include "ModelLoader.h"

//use hash table to save memory
//change lf to crlf
vector<float*> readAttributeOBJ(int count, ifstream* in, string endchr) {
	vector<float*> attributes;
	string _in;
	streampos endpos;

	while (_in != endchr) { *in >> _in; }
	while (true) {
		float* attribute = new float[count];
		for (int i = 0; i < count; i++) {
			*in >> attribute[i];
		}
		attributes.push_back(attribute);
		endpos = in->tellg();
		*in >> _in;

		if (_in != endchr) {
			in->seekg(endpos);
			break;
		}
	}
	return attributes;
}

void parseVertexStringOBJ(string vertex, int* indices) {
	string num;
	int c = 0;
	for (int i = 0; i < vertex.length(); i++) {
		if (vertex[i] == '/') {
			indices[c] = stoi(num);
			c++;
			num = "";
		}
		else {
			num += vertex[i];
		}
	}
	indices[c] = stoi(num);
}

void writeAttributeOBJ(vector<float>* n, vector<float*>* o, int i, int c, bool reverse = false) {
	for (int e = 0; e < c; e++) {
		if (reverse) 
			if ((e == 1) && (c == 2)) {
				n->push_back(1 - (*o)[i - 1][e]);
			}
			else {
				n->push_back((*o)[i - 1][e]);
			}
		else {
			n->push_back((*o)[i - 1][e]);
		}
	}
}

int calculateIndex(int numItems, int index, int itemIndex) {
	return (index * numItems + itemIndex);
}

float** loadModelFromOBJ(string fname, int* vCount, int groups, bool tangents) {
	ifstream inf;
	inf.open(fname);
	if (!inf) {
		applicationErrorCallback("Attempted to load model from obj, but " + fname + " is not a valid file name");
	}

	vector<float*> vp;
	vector<float*> vn;
	vector<float*> vt;
	vector<float>* fp = new vector<float>;
	vector<float>* fn = new vector<float>;
	vector<float>* ftangent = new vector<float>;
	vector<float>* ft = new vector<float>;
	vector<unsigned int>* fi = new vector<unsigned int>;

	vp = readAttributeOBJ(3, &inf, "v");
	vt = readAttributeOBJ(2, &inf, "vt");
	vn = readAttributeOBJ(3, &inf, "vn");

	string _in;
	while (_in != "f") { inf >> _in; }
	unsigned int c = 0;
	while ((_in == "f") && (inf.peek() != EOF)) {
		inf >> _in;
		unsigned int s = c;
		while ((_in.length() >= 5) && (inf.peek() != EOF)) {
			int indices[3];
			parseVertexStringOBJ(_in, indices);
			writeAttributeOBJ(fp, &vp, indices[0], 3);
			writeAttributeOBJ(ft, &vt, indices[1], 2, true);
			writeAttributeOBJ(fn, &vn, indices[2], 3);
			fi->push_back(c);
			c++;
			inf >> _in;
		}

		if (tangents) {
			//calculating tangent vectors for normal mapping
			float du1 = (*ft)[calculateIndex(2, s + 1, 0)] - (*ft)[calculateIndex(2, s, 0)];
			float dv1 = (*ft)[calculateIndex(2, s + 1, 1)] - (*ft)[calculateIndex(2, s, 1)];
			float du2 = (*ft)[calculateIndex(2, s + 2, 0)] - (*ft)[calculateIndex(2, s + 1, 0)];
			float dv2 = (*ft)[calculateIndex(2, s + 2, 1)] - (*ft)[calculateIndex(2, s + 1, 1)];
			glm::vec2 duv1 = glm::vec2(du1, dv1);
			glm::vec2 duv2 = glm::vec2(du2, dv2);

			glm::vec3 p1 = glm::vec3((*fp)[calculateIndex(3, s, 0)], (*fp)[calculateIndex(3, s, 1)], (*fp)[calculateIndex(3, s, 2)]);
			glm::vec3 p2 = glm::vec3((*fp)[calculateIndex(3, s + 1, 0)], (*fp)[calculateIndex(3, s + 1, 1)], (*fp)[calculateIndex(3, s + 1, 2)]);
			glm::vec3 p3 = glm::vec3((*fp)[calculateIndex(3, s + 2, 0)], (*fp)[calculateIndex(3, s + 2, 1)], (*fp)[calculateIndex(3, s + 2, 2)]);
			glm::vec3 dp1 = p2 - p1;
			glm::vec3 dp2 = p3 - p2;

			if (duv2.y == 0) {
				duv2.y = 1e-8;
			}
			glm::vec3 top = dp1 - dp2 * (duv1.y / duv2.y);
			float bottom = duv1.x - duv2.x * (duv1.y / duv2.y);
			if (bottom == 0) {
				bottom = 1e-8;
			}
			glm::vec3 tangent = top / bottom;

			for (int i = 0; i < 3; i++) {
				for (int f = 0; f < 3; f++) {
					ftangent->push_back(-1 * tangent[f]);
				}
			}
		}
	}

	inf.close();
	float** vertices;
	vertices = new float*[tangents ? 5: 4];
	vertices[0] = &(*fp)[0];
	vertices[1] = &(*fn)[0];
	vertices[2] = &(*ft)[0];
	if (tangents) {
		vertices[3] = &(*ftangent)[0];
		vertices[4] = (float*)&(*fi)[0];
	}
	else {
		vertices[3] = (float*)&(*fi)[0];
	}
	*vCount = c;
	
	cout << endl << "loaded model: " << fname << endl;
	cout << "vertex count: " << c << endl << endl;
	return vertices;
}