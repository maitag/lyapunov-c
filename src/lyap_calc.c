/* ------------- Lyapunov Generator ---------------
 * console based tool that renderes a lyapunov diagram
 * of a dynamic system and outputs image in ppm format
 *
 * by: Sylvio Sell , Rostock 1998
 *
*/

// ---------------------PROTOTYPEN ---------------------------------
#include <math.h>

typedef struct fknot
{
    double (*f_zeiger)();
    double constante;
    double *parameter;
    struct fknot *RZ;
    struct fknot *LZ;
} fknot;

double my_pow(double,double);
double paramfkt(fknot *knot_zeiger);
double constfkt(fknot *knot_zeiger);
double plus(fknot *knot_zeiger); //____________________________Addition
double minus(fknot *knot_zeiger);//____________________________Subtraktion
double mal(fknot *knot_zeiger);  //_____________________________Produkt
double durch(fknot *knot_zeiger);//____________________________Division
double hoch(fknot *knot_zeiger); //____________________________Potenz
double f_sin(fknot *knot_zeiger);//____________________________Sinus
double f_cos(fknot *knot_zeiger);//____________________________Cosinus
double f_tan(fknot *knot_zeiger);//____________________________Tangens
double f_cot(fknot *knot_zeiger);//____________________________Cotangens
double f_log(fknot *knot_zeiger);//____________________________Logarithmus
double f_ln(fknot *knot_zeiger); //____________________________Nat.Logarithm.
double f_abs(fknot *knot_zeiger);//____________________________AbsoluterBetrag


// --------------- DEKL Calculationsfkt ------------------------------------
// -------------------------------------------------------------------------

// eigene pow Fkt
double my_pow(double a,double x)
{ int i,j;
  double ergebnis,xyz;

  if (x==0)                  // x==0
    { ergebnis=1;
    }
  else 
    { if      (a==0)   { ergebnis=0;  }
      else if (a>0)
        {
            if  (floor(x)!=x) // gebrochener Exponent
               {  ergebnis=x*log(a)+1;
                  for (i=2;i<=30;i++)
                    {  xyz=1;for (j=1;j<=i;j++) xyz=xyz*j;
                       ergebnis+=pow(x*log(a),i)/xyz;
                    };
               }
             else             // ganzahliger Exponent
               {  if (x>0)  {  ergebnis=pow(a,x); }
                  else      {  ergebnis=pow(a,-x);
                               if (ergebnis!=0) ergebnis=1/ergebnis;
                            };
               };
        } 
      else  // (a<0)
        { if (floor(x)!=x)  // gebrochener Exponent
            { ergebnis=x*log(-a)+1;
              for (i=2;i<=30;i++)
                  {  xyz=1;for (j=1;j<=i;j++) xyz=xyz*j;
                     ergebnis+=pow(x*log(-a),i)/xyz;
                  };                
            }
          else             // ganzzahliger Exponent
            {  if (x>0) { ergebnis=pow(a,x); }
               else     { ergebnis=pow(a,-x);
                          if (ergebnis!=0) ergebnis=1/ergebnis;
                        };
            };
        };
    };
  return(ergebnis);
};

double paramfkt(fknot *knot_zeiger)
{ return( *(knot_zeiger->parameter) );
}

double constfkt(fknot *knot_zeiger)
{ return(knot_zeiger->constante);
}

double plus(fknot *knot_zeiger) //____________________________Addition
{ return( ((knot_zeiger->LZ)->f_zeiger(knot_zeiger->LZ))+((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ))  );
}

double minus(fknot *knot_zeiger) //___________________________Subtraktion
{ return( ((knot_zeiger->LZ)->f_zeiger(knot_zeiger->LZ))-((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ))  );
}

double mal(fknot *knot_zeiger) //_____________________________Produkt
{ return( ((knot_zeiger->LZ)->f_zeiger(knot_zeiger->LZ))*((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ))  );
}

double durch(fknot *knot_zeiger) //___________________________Division
{ if (((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ))==0) return(0);
  else return( ((knot_zeiger->LZ)->f_zeiger(knot_zeiger->LZ))/((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ))  );
}

double hoch(fknot *knot_zeiger) //____________________________Potenz
{  return( my_pow( ((knot_zeiger->LZ)->f_zeiger(knot_zeiger->LZ)),((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ))));
}

double f_sin(fknot *knot_zeiger) //___________________________Sinus
{ return(sin((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ)));
}

double f_cos(fknot *knot_zeiger) //___________________________Cosinus
{ return(cos((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ)));
}

double f_tan(fknot *knot_zeiger) //___________________________Tangens
{ return(tan((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ)));
}

double f_cot(fknot *knot_zeiger) //___________________________Cotangens
{ double x;
  x=tan((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ));
  if (x==0) return(0); else return(1/x);
}

double f_log(fknot *knot_zeiger) //___________________________Logarithmus
{ double x,y;
  x= ((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ));
  y= ((knot_zeiger->LZ)->f_zeiger(knot_zeiger->LZ));
  if ((x>0)&&(y>0))
    {  if (log(y)!=0) return( log(x)/log(y) );
       else return(0);
    }
  else  {  return(0); };
}

double f_ln(fknot *knot_zeiger) //____________________________Nat.Logarithm.
{ double x;

  x= ((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ));
  if (x>0) return(log(x));
  else     return(0);
}

double f_abs(fknot *knot_zeiger) //___________________________AbsoluterBetrag
{ return(fabs((knot_zeiger->RZ)->f_zeiger(knot_zeiger->RZ)));
}

