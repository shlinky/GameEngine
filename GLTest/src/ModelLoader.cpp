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

void writeAttributeOBJ(vector<float>* n, vector<float*>* o, int i, int c) {
	for (int e = 0; e < c; e++) {
		n->push_back((*o)[i - 1][e]);
	}
}


float** loadModelFromOBJ(string fname, int* vCount, int groups) {
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
		while ((_in.length() >= 5) && (inf.peek() != EOF)) {
			int indices[3];
			parseVertexStringOBJ(_in, indices);
			writeAttributeOBJ(fp, &vp, indices[0], 3);
			writeAttributeOBJ(ft, &vt, indices[1], 2);
			writeAttributeOBJ(fn, &vn, indices[2], 3);
			fi->push_back(c);
			c++;
			inf >> _in;
		}
	}

	inf.close();
	float** vertices = new float*[4];
	vertices[0] = &(*fp)[0];
	vertices[1] = &(*fn)[0];
	vertices[2] = &(*ft)[0];
	vertices[3] = (float*)&(*fi)[0];
	*vCount = c;
	
	cout << endl << "loaded model: " << fname << endl;
	cout << "vertex count: " << c << endl << endl;
	return vertices;
}