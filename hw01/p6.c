char a;
int main() {
	foo();
	// ...
}

int foo() {
	char * b, * c;
	char d;

	b = (char *)malloc(10);
	c = (char *)&a;
	d = 10;
	return d;
}
