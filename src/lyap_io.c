/* ------------- Lyapunov Generator ---------------
 * console based tool that renderes a lyapunov diagram
 * of a dynamic system and outputs image in ppm format
 *
 * by: Sylvio Sell , Rostock 1998
 *
*/

// ---------------------PROTOTYPEN ---------------------------------

double get_double();
int get_int();
int get_fkt(char *, fknot **, double* );
void print_fkt(fknot *, double* );

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
double get_double()
{ float a;
  
  if(scanf("%f",&a))
    {}
  else {printf("IO-ERROR, while getting double-parameter\n");
        a=0;}
  return ((double)a);
} //end get_double

// ---------------------------------------------------------------------
int get_int()
{ int a;
  
  if(scanf("%i",&a))
    {}
  else {printf("IO-ERROR, while getting integer-parameter\n");
        a=100;}
  return (a);
} //end get_int

// ---------------------------------------------------------------------
int get_fkt(char *desc, fknot **f, double* parfeld)
{   int k;
    char buffer[1024];
    char *anfang,*ende;

    printf(desc);

    if(scanf("%s",buffer))
      { //printf("\n# ");
        anfang=buffer;ende=anfang+(strlen(buffer)-1);

        k=parse_string(anfang,ende);
          if ( k==0 )
            { ende=anfang+(strlen(buffer)-1);
              *f=string_in_fkt(anfang,ende,parfeld);
            }
           else
            { printf("\n# %d ERRORs while parsing Function (CHECK Brackets & Parameter !!!)\n",k);
            };
      }
    else
      { printf("\n# I/O ERROR (Ljapunow...get_fkt\n");
        k=2;
      };
   return(k);
} // end get_fkt

// ---------------------------------------------------------------------
void print_fkt(fknot *k1,double* p)
{  if (((k1->f_zeiger)!=constfkt)&&((k1->f_zeiger)!=paramfkt)&&((k1->f_zeiger)!=f_ln)&&((k1->f_zeiger)!=f_abs)&&((k1->f_zeiger)!=f_sin)&&((k1->f_zeiger)!=f_cos)&&((k1->f_zeiger)!=f_tan)&&((k1->f_zeiger)!=f_cot))
        {  printf("(");
           print_fkt(k1->LZ,p);
        };
   if ((k1->f_zeiger)==plus)  printf("+");
   if ((k1->f_zeiger)==minus) printf("-");
   if ((k1->f_zeiger)==mal)   printf("*");
   if ((k1->f_zeiger)==durch) printf("/");
   if ((k1->f_zeiger)==hoch)  printf("^");
   if ((k1->f_zeiger)==f_sin) printf("sin(");
   if ((k1->f_zeiger)==f_cos) printf("cos(");
   if ((k1->f_zeiger)==f_tan) printf("tan(");
   if ((k1->f_zeiger)==f_cot) printf("cot(");
   if ((k1->f_zeiger)==f_log) printf(")log(");
   if ((k1->f_zeiger)==f_ln) printf("ln(");
   if ((k1->f_zeiger)==f_abs) printf("abs(");

   if ((k1->f_zeiger)==constfkt)
          { printf("%g",k1->constante);
          }
   else
         { if ((k1->f_zeiger)==paramfkt)
            { printf("%c",(char)((k1->parameter)-p+97));
            }
          else
            {  print_fkt(k1->RZ,p);
            };
         };
   if (((k1->f_zeiger)!=constfkt)&&((k1->f_zeiger)!=paramfkt)) printf(")");

} //end print_tree  ---------------------------------
