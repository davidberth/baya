export module test;

export class Test {
	int a;
public:
	Test() {
		a = 10;
	}

	
	~Test() {}
	
	const int getA() const {
		return a;
	}
};