int main() {
	double w = 1234.789;
	double x = 0;
	double y;
	double * z;

	/* (1) */ z = (double *)&w
	/* (2) */ scanf("%f", &y);
	/* (3) */ *z = sqrt(w);
	/* (4) */ y = y*2.0;
	/* (5) */ printf("y=%f, *z=%f\n", y, *z);
	/* (6) */ y = y/x;
	/* (7) */ w = y*z;
	return 0;
}
