/* ------------- Lyapunov Generator ---------------
 * console based tool that renderes a lyapunov diagram
 * of a dynamic system and outputs image in ppm format
 *
 * by: Sylvio Sell , Rostock 1998
 *
 * changelog:
 * 2000 - coloring functions
 * 2017 - code cleaning, git
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h>

#include "lyap_calc.c"
#include "lyap_math.c"
#include "lyap_io.c"

int main(argc,argv)
  int argc;
  char *argv[];
{
	double a_start, b_start, a_end, b_end;
	int itervor, iter, i;
	double startiter;
	fknot *funct_a, *ableit_a;
	fknot *funct_b, *ableit_b;
	fknot *funct_red;
	fknot *funct_green;
	fknot *funct_blue;
	double p[25];

	unsigned char *speicher,*sp;
	unsigned char r,g,b;
	unsigned int breite,hoehe;
	
	int ispipe = 0;
	
	// --------- check params    -----------------------------------------
	char *modus;

	printf("P3\n"); // header for ppm image format
	
	for (i=1; i<argc; i++)
	{
		if (strcmp(argv[i], "-p") == 0) ispipe = 1;
	}
	
	printf("# Ljapunow-Generator - by Sylvio Sell, 1998\n");
	printf("# -----------------------------------------\n\n");

	// --------- function input of dynamic system ------------------------------
	printf("# function of dynamic system, needs parameters: x , a\n");
	if (get_fkt("# f (x) = ", &funct_a,p)!=0) exit(0);
	if (ispipe) { print_fkt(funct_a,p); printf("\n"); }
	funct_a = kuerzen(funct_a);  //printf("\n# fa(x)="); print_fkt(funct_a,p);
	funct_b = copy_fkt(funct_a); //printf("\n# fb(x)="); print_fkt(funct_b,p);

	swap_fkt_par(funct_b, &p[0], &p[1]); //printf("\n# fba(x)="); print_fkt(funct_b,p);

	ableit_a=kuerzen(abl(funct_a, &p[23]));
	printf("# f'(x) = "); print_fkt(ableit_a, p);
	ableit_b=kuerzen(abl(funct_b,&p[23]));

	printf("\n\n");
	
	// ---------- function input for coloring -----------------------------------
	printf("# function for red color, needs parameter: i\n");
	if (get_fkt("# r(i) = ", &funct_red,p)!=0) exit(0);
	if (ispipe) { print_fkt(funct_red,p); printf("\n"); }
	funct_red = kuerzen(funct_red);

	if (get_fkt("# g(i) = ", &funct_green,p)!=0) exit(0);
	if (ispipe) { print_fkt(funct_green,p); printf("\n"); }
	funct_green = kuerzen(funct_green);

	if (get_fkt("# b(i) = ", &funct_blue,p)!=0) exit(0);
	if (ispipe) { print_fkt(funct_blue,p); printf("\n"); }
	funct_blue = kuerzen(funct_blue);
	
	printf("\n");

	// ---------- calculate lyapunov diagram  ---------------------------------
	printf("# width :"); breite=get_int();   if (ispipe) printf(" %i\n",breite);
	printf("# height:"); hoehe=get_int();    if (ispipe) printf(" %i\n",hoehe);
	printf("# a-min:"); a_start=get_double();if (ispipe) printf(" %g\n",a_start);
	printf("# a-max:"); a_end=get_double();  if (ispipe) printf(" %g\n",a_end);
	printf("# b-min:"); b_start=get_double();if (ispipe) printf(" %g\n",b_start);
	printf("# b-max:"); b_end=get_double();  if (ispipe) printf(" %g\n",b_end);
	printf("# start value:"); startiter=get_double(); if (ispipe) printf(" %g\n",startiter);
	printf("# Number of pre-iterations:"); itervor=get_int(); if (ispipe) printf(" %i\n",itervor);
	printf("# Number of iterations to simmulate dynamic system:"); iter=get_int(); if (ispipe) printf(" %i\n",iter);
	
	printf("\n%i %i", breite, hoehe);
	printf("\n255\n");

	speicher = (unsigned char *) malloc(breite*hoehe*sizeof(double));
	lyapunov(speicher, funct_a, ableit_a, funct_b, ableit_b, p, a_start, a_end, b_start, b_end, startiter, breite, hoehe, itervor, iter);
	sp=speicher;
	double tmp;
	for (i=0; i<(breite*hoehe); i++)
	{
		memcpy(&p[8], sp, sizeof(double));
		sp+=sizeof(double);

		tmp = funct_red->f_zeiger(funct_red);
		if (tmp > 255.0)    r = 255;
		else if (tmp < 0.0) r = 0;
		else r = (unsigned char) (tmp);
		
		tmp = funct_green->f_zeiger(funct_green);
		if (tmp > 255.0)    g = 255;
		else if (tmp < 0.0) g = 0;
		else g = (unsigned char) (tmp);
		
		tmp = funct_blue->f_zeiger(funct_blue);
		if (tmp > 255.0)    b = 255;
		else if (tmp < 0.0) b = 0;
		else b = (unsigned char) (tmp);

		printf("%i %i %i    # %f \n",r,g,b,p[8]);
	}

	del_fkt(funct_a);del_fkt(ableit_a);
	del_fkt(funct_b);del_fkt(ableit_b);
	del_fkt(funct_red);del_fkt(funct_green);del_fkt(funct_blue);
}
