struct CNode {
	char* data;
	CNode *next;
};

struct Perem {
	char ch;
	double val;
	Perem* next;
};

CNode* Add(CNode*, char*);  

class Polish {
	Perem* x;
	int prioritet(char);
	bool isoperation(char);
	bool isStr(char);
	CNode* perevod(char*);  
public:
	char* Pols;
	Polish(char*);  

	void AddX(char, int); 
	double Culc();  
};
