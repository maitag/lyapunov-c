/* ------------- Lyapunov Generator ---------------
 * console based tool that renderes a lyapunov diagram
 * of a dynamic system and outputs image in ppm format
 *
 * by: Sylvio Sell , Rostock 1998
 *
*/

// ---------------------PROTOTYPEN ---------------------------------

fknot *set_fkt_fkt(double (*)(fknot*) ); //erzeugt Knoten mit Zeiger auf CalcFkt ohne Parameter
fknot *set_fkt_con(double); // erzeugt Knoten und speichert dort constante 
fknot *set_fkt_par(double *); // erzeugt Knoten und speichert ParameterZeiger
fknot *copy_fkt(fknot *);  // erzeugt Copie von Knoten samt Unterbauemen
void del_fkt(fknot *);  //  loescht Knoten und alle Unterbaueme
int parse_string(char *,char *);
fknot *string_in_fkt(char *,char *,double*);
void swap_fkt_par(fknot*,double*,double*);
fknot *abl(fknot *,double *);
fknot *kuerzen(fknot* );
int lyapunov(unsigned char*,fknot*,fknot*,fknot*,fknot*,double*,double,double,double,double,double,unsigned int,unsigned int,int,int);


//------------------------------------------------------------------------------
//----zum Erzeugen,Loeschen,Kopieren usw. der Fkt-Knoten bzw. Untertrees--------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

fknot *set_fkt_fkt(double (*funktionzeiger)(fknot*) ) //erzeugt Knoten mit Zeiger 
  { fknot *knotenzeiger;                              //auf CalcFkt ohne Parameter
    knotenzeiger=malloc(sizeof(fknot));
    knotenzeiger->f_zeiger=funktionzeiger;
    knotenzeiger->RZ=0;
    knotenzeiger->LZ=0;
    return (knotenzeiger);
  }  //-----------------END set_fkt_fkt

//------------------------------------------------------------------------------

fknot *set_fkt_con(double con)  // erzeugt Knoten und speichert dort constante 
  { fknot *knotenzeiger;
    knotenzeiger=malloc(sizeof(fknot));
    knotenzeiger->f_zeiger=constfkt;
    knotenzeiger->constante=con;
    knotenzeiger->LZ=0;
    return (knotenzeiger);
  }  //-----------------END set_fkt_con

//------------------------------------------------------------------------------

fknot *set_fkt_par(double *par)   // erzeugt Knoten und speichert ParameterZeiger
  { fknot *knotenzeiger;
    knotenzeiger=malloc(sizeof(fknot));
    knotenzeiger->f_zeiger=paramfkt;
    knotenzeiger->parameter=par;
    knotenzeiger->LZ=0;
    return (knotenzeiger);
  }  //-----------------END set_fkt_par

//------------------------------------------------------------------------------


fknot *copy_fkt(fknot *funct)  // erzeugt Copie von Knoten samt Unterbauemen

  { fknot *fxy;

    if ( ((funct->f_zeiger)!=constfkt)&&((funct->f_zeiger)!=paramfkt) )
      { fxy=set_fkt_fkt(funct->f_zeiger);
        (fxy->RZ)=copy_fkt(funct->RZ);
        (fxy->LZ)=copy_fkt(funct->LZ);
      }
    else
      { if ((funct->f_zeiger)==constfkt) 
           fxy=set_fkt_con(funct->constante);
        if ((funct->f_zeiger)==paramfkt) 
           fxy=set_fkt_par(funct->parameter);
      };
    return (fxy);
  }   // ----------------END copy_fkt-------------------------------------------

//------------------------------------------------------------------------------

void del_fkt(fknot *knotzeig)  //  loescht Knoten und alle Unterbaueme

  { if (((knotzeig->f_zeiger)!=constfkt)&&((knotzeig->f_zeiger)!=paramfkt))
      {
         del_fkt(knotzeig->LZ);
         del_fkt(knotzeig->RZ);
      };

    free(knotzeig);

  }   //------------------END del-fkt ---------------------------------------------



// -----------------PARSE_STRING------------------
int parse_string(char *s_first,char *s_last)
{ int i,klammer,fehler;
  char *s1,*s2,*s_merk;
  char a,a1;

  fehler=0;

  klammer=0;
  for (s1=s_first;s1<=s_last;s1++)
    { if ((*s1)=='(') klammer++;
      if ((*s1)==')') klammer--;
    }
  if (klammer!=0) fehler++;

  for (s1=s_first;s1<=s_last;s1++)
    {
       if ( ((*s1)!='.')&&((*s1)!='0')&&((*s1)!='1')&&((*s1)!='2')&&((*s1)!='3')&&((*s1)!='4')&&((*s1)!='5')&&((*s1)!='6')&&((*s1)!='7')&&((*s1)!='8')&&((*s1)!='9')  )
         {if ( ((*s1)!='(')&&((*s1)!=')')&&((*s1)!='+')&&((*s1)!='-')&&((*s1)!='*')&&((*s1)!='/')&&((*s1)!='^')  )
          {if ((isalpha(*s1))&&(!(isalpha(*(s1+1)))&&( (s1==s_first)|| ((s1!=s_first)&&(!(isalpha(*(s1-1)))))   ) ) )
             {//printf("# Parameter:%c\t\n",*s1);
             }

           else
            { if ( ((*s1)=='s')||((*s1)=='S')  )
                 { if ( ((*(s1+1)=='i')||(*(s1+1)=='I')  )&&( (*(s1+2)=='n')||(*(s1+2)=='N')  )  )
                        { s1++;s1++; }
                   else { fehler++;  };
                 }
              else //1
                 { if ( ((*s1)=='t')||((*s1)=='T')  )
                      { if ( ( (*(s1+1)=='a')||(*(s1+1)=='A')  )&&( (*(s1+2)=='n')||(*(s1+2)=='N')) )
                             { s1++;s1++; }
                        else { fehler++;  };
                      }
                   else //2
                      { if ( ((*s1)=='c')||((*s1)=='C')  )
                           { if ( ( (*(s1+1)=='o')||(*(s1+1)=='O')  )&&( ((*(s1+2)=='t')||(*(s1+2)=='T')  )||((*(s1+2)=='s')||(*(s1+2)=='S')  ))  )
                                  { s1++;s1++; }
                             else { fehler++;  };
                           }
                        else //3
                           { if ( ((*s1)=='l')||((*s1)=='L')  )
                                { if ( ( (*(s1+1)=='o')||(*(s1+1)=='O')  )&&( ((*(s1+2)=='g')||(*(s1+2)=='G')  )||((*(s1+2)=='s')||(*(s1+2)=='S')  ))  )
                                       { s1++;s1++; }
                                  else { if ((*(s1+1)=='n')||(*(s1+1)=='N'))
                                             { s1++;     }
                                         else  { fehler++; };
                                       };
                                 }
                             else  { if ( ((*s1)=='a')||((*s1)=='A')  )
                                        { if  (((*(s1+1)=='b')||(*(s1+1)=='B')  )&&( (*(s1+2)=='s')||(*(s1+2)=='S') ) )
                                              { s1++;s1++; }
                                         else { fehler++;  };
                                        }
                                     else
                                        {  fehler++;
                                        };
                                   };
                           }; //end else 3
                      };//end else 2
                 }; //end else 1
               };
            };// end boddy if 2
         }; //end boddy if 1
    };//end for

     for (s1=s_first;s1<=s_last;s1++)
      {  if ( ( ((*s1)=='+')||((*s1)=='-') )&&( (s1==s_first)||((*(s1-1))=='+')||((*(s1-1))=='-')||((*(s1-1))=='*')||((*(s1-1))=='/')||((*(s1-1))=='^')||((*(s1-1))=='n')||((*(s1-1))=='N')||((*(s1-1))=='s')||((*(s1-1))=='S')||((*(s1-1))=='g')||((*(s1-1))=='G')||((*(s1-1))=='t')||((*(s1-1))=='T')))
            { a=*s1;
              *s1='(';s1++;s_last++;
                for (s2=s1;s2<=s_last+1;s2++)   { a1=*s2;*s2=a;a=a1;  }
                 s_merk=s1;
                 while (((*s1)=='+')||((*s1)=='-')||( (isalpha(*s1))&&(isalpha(*(s1+1))) )||( (isalpha(*s1))&&(isalpha(*(s1-1)))&&(s1>s_first) ))
                     {  s1++; }


                 if ((*s1)=='(')
                   { klammer=1;
                     for (s1=s1+1;(klammer!=0)&&(s1<=s_last);s1++)
                       { if ((*s1)=='(') klammer++;
                         if ((*s1)==')') klammer--;
                       };
                   }
                else { if ( (isalpha(*s1))&&(!(isalpha(*(s1+1)))) )
                          s1++;
                       else
                         while ( (s1<=s_last)&&(  ((*s1)=='.')||((*s1)=='0')||((*s1)=='1')||((*s1)=='2')||((*s1)=='3')||((*s1)=='4')||((*s1)=='5')||((*s1)=='6')||((*s1)=='7')||((*s1)=='8')||((*s1)=='9')) )
                              {  s1++; };
                     };
                 a=*s1;
                 *s1=')';

                for (s2=s1+1;s2<=s_last+1;s2++)   { a1=*s2;*s2=a;a=a1;  }
                  s1=s_merk;
              }
         }
  return (fehler);
  } // end parse_string  --------------------------------------------

fknot *string_in_fkt(char *s_first,char *s_last,double* parfeld)
{  fknot *f1;       // STRING in FUNCTION ---------------------------------
   char a;
   char  *s1,*op1,*op2;
   int klammer;
  
  klammer=0;

  op1=s_last;op2=s_last; // ---------------zeigen auf anf.und ende der gesuchten OPERATION (am anfang s_last)
  for (s1=s_first;s1<=s_last;s1++)
   { 
     if (klammer>0)
       switch(*s1)
         { case '(': klammer++; break;
           case ')': klammer--;  break;
         }
     else
       switch(*s1)
         { case '(' : klammer++;break;
           case '+' :
           case '-' : op1=s1; op2=op1;break;
           case '*' :
           case '/' : if (  ( (*op1)!='+' )&&( (*op1)!='-')&&( (*op1)!='*')&&( (*op1)!='/')   )
                         { op1=s1; op2=op1; }
                      break;
           case '^' : if (  ( (*op1)!='+' )&&( (*op1)!='-')&&( (*op1)!='*')&&( (*op1)!='/')&&( (*op1)!='^') )
                         { op1=s1; op2=op1; }
                      break;
           case 's' :
           case 'S' : if ( (op1==s_last)&&( ((*(s1+1))=='i')||((*(s1+1))=='I') )&&( ((*(s1+2))=='n')||((*(s1+2))=='N')) )
                         { op1=s1;s1++;s1++;op2=s1;  };
                      break;
           case 'c' :
           case 'C' : if ( (op1==s_last)&&( ((*(s1+1))=='o')||((*(s1+1))=='O')) )
                         { if ( (op1==s_last)&&( ((*(s1+2))=='s')||((*(s1+2))=='S')||((*(s1+2))=='t')||((*(s1+2))=='T')) )
                              { op1=s1;s1++;s1++;op2=s1;  };
                         };
                      break;
           case 't' :
           case 'T' : if ( (op1==s_last)&&( ((*(s1+1))=='a')||((*(s1+1))=='A') )&&( ((*(s1+2))=='n')||((*(s1+2))=='N')) )
                         { op1=s1;s1++;s1++;op2=s1;  };
                      break;
           case 'l' :
           case 'L' : if ( (op1==s_last)&&( ((*(s1+1))=='o')||((*(s1+1))=='O'))&&( ((*(s1+2))=='g')||((*(s1+2))=='G')) )
                         { op1=s1;s1++;s1++;op2=s1;  }
                      else { if ( (op1==s_last)&&(  ((*(s1+1))=='n')||((*(s1+1))=='N')))
                                { op1=s1;s1++;op2=s1;  };
                           };
                      break;
           case 'a' :
           case 'A' : if ( (op1==s_last)&&( ((*(s1+1))=='b')||((*(s1+1))=='B') )&&( ((*(s1+2))=='s')||((*(s1+2))=='S')) )
                         { op1=s1;s1++;s1++;op2=s1;  };
                      break;

         }; // end switch und else
   }; //end for()
  if (op1!=s_last)  // operation where found
     {      f1=malloc(sizeof(fknot));

            if (op2==s_last) // kein rechter opperant
               {  (f1->RZ)=set_fkt_con(0);
               }
            else
               { (f1->RZ)=string_in_fkt(op2+1,s_last,parfeld);
               };

            if (op1==s_first) // kein linker opperant
               {  (f1->LZ)=set_fkt_con(0);
               }
            else
               { if (  ( (((*op1)=='l')||((*op1)=='L'))&&(((*op2)=='g')||((*op2)=='G')) ) ||((*op1)=='+')||((*op1)=='-')||((*op1)=='*')||((*op1)=='/')||((*op1)=='^')  )
                      (f1->LZ)=string_in_fkt(s_first,op1-1,parfeld);
                 else (f1->LZ)=set_fkt_con(0);
               };

            switch (*op1)
               { case '+': (f1->f_zeiger)=plus;break;
                 case '-': (f1->f_zeiger)=minus;break;
                 case '*': (f1->f_zeiger)=mal;break;
                 case '/': (f1->f_zeiger)=durch;break;
                 case '^': (f1->f_zeiger)=hoch;break;
                 case 's':
                 case 'S': if ( ( ((*(op1+1))=='i')||((*(op1+1))=='I') )&&( ((*(op1+2))=='n')||((*(op1+2))=='N')) )
                              { (f1->f_zeiger)=f_sin;  };
                           break;
                 case 'c':
                 case 'C': if ( ((*(op1+1))=='o')||((*(op1+1))=='O')   )
                             { if ( ((*(op1+2))=='s')||((*(op1+2))=='S')   )
                                  { (f1->f_zeiger)=f_cos; }
                               else
                                  { (f1->f_zeiger)=f_cot; };
                             };
                           break;
                 case 't':
                 case 'T': if ( (((*(op1+1))=='a')||((*(op1+1))=='A'))&&(((*(op1+2))=='n')||((*(op1+2))=='N')) )
                              { (f1->f_zeiger)=f_tan;  };
                           break;
                 case 'l':
                 case 'L': if ( (((*(op1+1))=='o')||((*(op1+1))=='O'))&&(((*(op1+2))=='g')||((*(op1+2))=='G')) )
                              { (f1->f_zeiger)=f_log;  }
                           else
                              { if (((*(op1+1))=='n')||((*(op1+1))=='N'))
                                   { (f1->f_zeiger)=f_ln; };
                              };
                           break;
                 case 'a':
                 case 'A': if ( ( ((*(op1+1))=='b')||((*(op1+1))=='B'))&&(((*(op1+2))=='s')||((*(op1+2))=='S')) )
                              { (f1->f_zeiger)=f_abs;  };
                           break;

               };//end switch
               return(f1);


     }//end if operation found???-Block
  else         // no operat. found
     { if ( ((*s_last)==')')&&((*s_first)=='(') )
          { return(string_in_fkt(s_first+1,s_last-1,parfeld));
          }
       else
          { if ((s_first==s_last)&&(isalpha(*s_first)))
            { 
               f1=set_fkt_par(&parfeld[(int)(tolower(*s_first))-97] );
            }
            else
             { a=*(s_last+1);*(s_last+1)='\0';
               f1=set_fkt_con(atof(s_first));
               *(s_last+1)=a;
             };
               return (f1);
          };
     };//end all if

} //end string_in_fkt -----------string_in_fkt------string_in_fkt----


//----------------------------------------------------------------------------
void swap_fkt_par(fknot* f,double* p1,double* p2)
{ if ((f->f_zeiger)==paramfkt)
    { if ((f->parameter)==p1) { (f->parameter)=p2; }
        else  {  if ((f->parameter)==p2) (f->parameter)=p1;  }
    }
  else
    { if ((f->f_zeiger)!=constfkt)
        {  swap_fkt_par(f->RZ,p1,p2);swap_fkt_par(f->LZ,p1,p2);
        }
    };

}


//----------------  ABLEITUNG -----------------------------------------
fknot *abl(fknot *funct,double *wonach)
{ fknot *ableit;

  if ( ((funct->f_zeiger)==plus)||((funct->f_zeiger)==minus)  )
    {
     ableit=set_fkt_fkt(funct->f_zeiger);
     (ableit->RZ)=abl(funct->RZ,wonach);
     (ableit->LZ)=abl(funct->LZ,wonach);
    }
  else  // else 1
    {
     if ((funct->f_zeiger)==mal)
       {
        ableit=set_fkt_fkt(plus);
          (ableit->RZ)=set_fkt_fkt(mal);
            ((ableit->RZ)->RZ)=abl(funct->RZ,wonach);
            ((ableit->RZ)->LZ)=copy_fkt(funct->LZ);
          (ableit->LZ)=set_fkt_fkt(mal);
            ((ableit->LZ)->RZ)=copy_fkt(funct->RZ);
            ((ableit->LZ)->LZ)=abl(funct->LZ,wonach);
       }
     else    // else 2
       {
        if ((funct->f_zeiger)==durch)
          {
           ableit=set_fkt_fkt(durch);

           (ableit->RZ)=set_fkt_fkt(hoch);
           ((ableit->RZ)->RZ)=set_fkt_con(2);
           ((ableit->RZ)->LZ)=copy_fkt(funct->RZ);

           (ableit->LZ)=set_fkt_fkt(minus);

           ((ableit->LZ)->RZ)=set_fkt_fkt(mal);
           (((ableit->LZ)->RZ)->RZ)=abl(funct->RZ,wonach);
           (((ableit->LZ)->RZ)->LZ)=copy_fkt(funct->LZ);

           ((ableit->LZ)->LZ)=set_fkt_fkt(mal);
           (((ableit->LZ)->LZ)->RZ)=copy_fkt(funct->RZ);
           (((ableit->LZ)->LZ)->LZ)=abl(funct->LZ,wonach);
          }
        else //else 3
          {
           if ((funct->f_zeiger)==hoch)
             {
              ableit=set_fkt_fkt(mal);
                (ableit->RZ)=abl(funct->LZ,wonach);
                (ableit->LZ)=set_fkt_fkt(mal);
                  ((ableit->LZ)->RZ)=set_fkt_fkt(hoch);
                    (((ableit->LZ)->RZ)->RZ)=set_fkt_fkt(minus);
                      ((((ableit->LZ)->RZ)->RZ)->RZ)=set_fkt_con(1);
                      ((((ableit->LZ)->RZ)->RZ)->LZ)=copy_fkt(funct->RZ);
                    (((ableit->LZ)->RZ)->LZ)=copy_fkt(funct->LZ);
                  ((ableit->LZ)->LZ)=copy_fkt(funct->RZ);
             }
           else //else 4
             {
              if ((funct->f_zeiger)==constfkt)
                {
                 ableit=set_fkt_con(0);
                }
              else //else 5
                {
                 if ((funct->f_zeiger)==paramfkt)
                   {
                    if ((funct->parameter)==wonach) ableit=set_fkt_con(1);
                    else ableit=set_fkt_con(0);
                   };
                };  // end else 5
             };  // end else 4
          };  // end else 3
       }; //end else 2
    }; //end else 1

  if ( (funct->f_zeiger)==f_sin )
    {
      ableit=set_fkt_fkt(mal);
        (ableit->RZ)=abl(funct->RZ,wonach);
        (ableit->LZ)=set_fkt_fkt(f_cos);
          ((ableit->LZ)->LZ)=set_fkt_con(0);
          ((ableit->LZ)->RZ)=copy_fkt(funct->RZ);
    }
  else  // else 1
    {
     if ((funct->f_zeiger)==f_cos)
       {
        ableit=set_fkt_fkt(mal);
         (ableit->RZ)=abl(funct->RZ,wonach);
         (ableit->LZ)=set_fkt_fkt(minus);
          ((ableit->LZ)->LZ)=set_fkt_con(0);
          ((ableit->LZ)->RZ)=set_fkt_fkt(f_sin);
           (((ableit->LZ)->RZ)->LZ)=set_fkt_con(0);
           (((ableit->LZ)->RZ)->RZ)=copy_fkt(funct->RZ);

       }
     else    // else 2
       {
        if ((funct->f_zeiger)==f_tan)
          {
           ableit=set_fkt_fkt(mal);
            (ableit->RZ)=abl(funct->RZ,wonach);
            (ableit->LZ)=set_fkt_fkt(plus);
             ((ableit->LZ)->LZ)=set_fkt_con(1);
             ((ableit->LZ)->RZ)=set_fkt_fkt(hoch);
              (((ableit->LZ)->RZ)->RZ)=set_fkt_con(2);
              (((ableit->LZ)->RZ)->LZ)=set_fkt_fkt(f_tan);
                ((((ableit->LZ)->RZ)->LZ)->LZ)=set_fkt_con(0);
                ((((ableit->LZ)->RZ)->LZ)->RZ)=copy_fkt(funct->RZ);
          }
        else //else 3
          {
           if ((funct->f_zeiger)==f_cot)
             {
              ableit=set_fkt_fkt(mal);
                (ableit->RZ)=abl(funct->RZ,wonach);
                (ableit->LZ)=set_fkt_fkt(plus);
                 ((ableit->LZ)->LZ)=set_fkt_con(-1);
                 ((ableit->LZ)->RZ)=set_fkt_fkt(hoch);
                  (((ableit->LZ)->RZ)->RZ)=set_fkt_con(2);
                  (((ableit->LZ)->RZ)->LZ)=set_fkt_fkt(f_cot);
                    ((((ableit->LZ)->RZ)->LZ)->LZ)=set_fkt_con(0);
                    ((((ableit->LZ)->RZ)->LZ)->RZ)=copy_fkt(funct->RZ);
             }
           else //else 4
             {
              if ((funct->f_zeiger)==f_log)
                {
                  ableit=set_fkt_fkt(mal);
                  (ableit->RZ)=abl(funct->RZ,wonach);
                  (ableit->LZ)=set_fkt_fkt(durch);
                   ((ableit->LZ)->LZ)=set_fkt_con(1);
                   ((ableit->LZ)->RZ)=set_fkt_fkt(mal);
                    (((ableit->LZ)->RZ)->LZ)=copy_fkt(funct->RZ);
                    (((ableit->LZ)->RZ)->RZ)=set_fkt_fkt(f_ln);
                      ((((ableit->LZ)->RZ)->RZ)->LZ)=set_fkt_con(0);
                      ((((ableit->LZ)->RZ)->RZ)->RZ)=copy_fkt(funct->LZ);

                }
              else //else 5
                {
                 if ((funct->f_zeiger)==f_ln)
                   {
                     ableit=set_fkt_fkt(mal);
                      (ableit->RZ)=abl(funct->RZ,wonach);
                      (ableit->LZ)=set_fkt_fkt(durch);
                        ((ableit->LZ)->LZ)=set_fkt_con(1);
                        ((ableit->LZ)->RZ)=copy_fkt(funct->RZ);
                   }
                 else //else 6
                   {


                   };  // end else 6
                };  // end else 5
             };  // end else 4
          };  // end else 3
       }; //end else 2
    }; //end else 1

  return(ableit);
}

//-----------------------------------------------------------------------

fknot *kuerzen(fknot* f)
{  fknot *fk,*fk1,*fback;

   if ((f->f_zeiger)==plus) 
      {   fk=kuerzen(f->LZ);
          if (  ( (fk->f_zeiger)==constfkt)&&( (fk->constante)==0)  )
            { del_fkt(fk);
              fback=kuerzen(f->RZ); 
            }
          else
            { fk1=kuerzen(f->RZ);
              if (   ( (fk1->f_zeiger)==constfkt)&&( (fk1->constante)==0)  )
                {  del_fkt(fk1);
                   fback=fk;
                }
               else
                {  if (   ( (fk->f_zeiger)==constfkt)&&( (fk1->f_zeiger)==constfkt)  )
                     { fback=set_fkt_con((fk->constante)+(fk1->constante));
                       del_fkt(fk);
                       del_fkt(fk1);
                     }
                   else
                     { fback=set_fkt_fkt(plus);
                       (fback->LZ)=fk;
                       (fback->RZ)=fk1;
                     };
                };
            };
      };

   if ((f->f_zeiger)==minus) 
      {   fk=kuerzen(f->LZ);
          fk1=kuerzen(f->RZ);
          if (   ( (fk1->f_zeiger)==constfkt)&&( (fk1->constante)==0)  )
             {  del_fkt(fk1);
                fback=fk;
             }
           else
             {  if (   ( (fk->f_zeiger)==constfkt)&&( (fk1->f_zeiger)==constfkt)  )
                   { fback=set_fkt_con((fk->constante)-(fk1->constante));
                     del_fkt(fk);
                     del_fkt(fk1);
                   }
                else
                   { fback=set_fkt_fkt(minus);
                     (fback->LZ)=fk;
                     (fback->RZ)=fk1;
                   };
             };
      };

   if ((f->f_zeiger)==mal) 
      { fk=kuerzen(f->LZ);
        if (  ( (fk->f_zeiger)==constfkt)&&( (fk->constante)==0)  )
          { del_fkt(fk);
            fback=set_fkt_con(0);
          }
        else
          { if (  ( (fk->f_zeiger)==constfkt)&&( (fk->constante)==1)  )
              { del_fkt(fk);
                fback=kuerzen(f->RZ); 
              }
            else
              { fk1=kuerzen(f->RZ);
                if (  ( (fk1->f_zeiger)==constfkt)&&( (fk1->constante)==0)  )
                   { del_fkt(fk1);
                     del_fkt(fk);
                     fback=set_fkt_con(0);
                   }
                else
                   {  if (   ( (fk1->f_zeiger)==constfkt)&&( (fk1->constante)==1)  )
                         {  del_fkt(fk1);
                            fback=fk;
                         }
                      else
                         { if (   ( (fk->f_zeiger)==constfkt)&&( (fk1->f_zeiger)==constfkt)  )
                             { fback=set_fkt_con((fk->constante)*(fk1->constante));
                               del_fkt(fk);
                               del_fkt(fk1);	
                             }
                           else
                             { fback=set_fkt_fkt(mal);
                               (fback->LZ)=fk;
                               (fback->RZ)=fk1;
                             }; 
                         };
                   };
              };
          };
      };

   if ((f->f_zeiger)==durch) 
      {   fk=kuerzen(f->LZ);
          if (   ( (fk->f_zeiger)==constfkt)&&( (fk->constante)==0)  )
             {  del_fkt(fk);
                fback=set_fkt_con(0);
             }
           else
             { fk1=kuerzen(f->RZ);
               if (   ( (fk1->f_zeiger)==constfkt)&&( (fk1->constante)==1)  )
                  {  del_fkt(fk1);
                     fback=fk;
                  }
                else
                  {  if (   ( (fk->f_zeiger)==constfkt)&&( (fk1->f_zeiger)==constfkt)  )
                        { fback=set_fkt_con((fk->constante)/(fk1->constante));
                          del_fkt(fk);
                          del_fkt(fk1);	
                        }
                     else
                        {
                          fback=set_fkt_fkt(durch);
                          (fback->LZ)=fk;
                          (fback->RZ)=fk1;
                        };
                  };
             };
      };

   if ((f->f_zeiger)==hoch) 
      {   fk=kuerzen(f->LZ);
          fk1=kuerzen(f->RZ);
          if (   ( (fk1->f_zeiger)==constfkt)&&( (fk1->constante)==0)  )
             {  del_fkt(fk1);
                del_fkt(fk);
                fback=set_fkt_con(1);
             }
           else
             {  if (  ( ((fk->f_zeiger)==constfkt)&&((fk1->constante)==1) )||( ((fk1->f_zeiger)==constfkt)&&((fk1->constante)==1) )  )
                  {  del_fkt(fk1);
                     fback=fk;
                  }
                else
                  { if (   ( (fk->f_zeiger)==constfkt)&&( (fk1->f_zeiger)==constfkt)  )
                       { fback=set_fkt_con(my_pow(  (fk->constante),(fk1->constante)  ));
                         del_fkt(fk);
                         del_fkt(fk1);	
                       }
                     else { fback=set_fkt_fkt(hoch);
                            (fback->LZ)=fk;
                            (fback->RZ)=fk1;
                          };
                  };
             };
      };


  if ((f->f_zeiger)==f_ln)
      {   fk1=kuerzen(f->RZ);
          if (   ( (fk1->f_zeiger)==constfkt)&&( (fk1->constante)==1)  )
             {  del_fkt(fk1);
                fback=set_fkt_con(0);
             }
           else
             {  fback=set_fkt_fkt(f_ln);
                (fback->LZ)=set_fkt_con(0);
                (fback->RZ)=fk1;
             };
      };

  if ((f->f_zeiger)==f_log)
      {   fk=kuerzen(f->LZ);
          fk1=kuerzen(f->RZ);
          if (   ( (fk1->f_zeiger)==constfkt)&&( (fk1->constante)==1)  )
             {  del_fkt(fk1);
                del_fkt(fk);
                fback=set_fkt_con(0);
             }
           else
             {  fback=set_fkt_fkt(f_log);
                (fback->LZ)=fk;
                (fback->RZ)=fk1;
             };
      };

   if ( ((f->f_zeiger)==paramfkt)||((f->f_zeiger)==constfkt) )
      {  fback=copy_fkt(f);
      };

   if ( ((f->f_zeiger)==f_abs)||((f->f_zeiger)==f_cos)||((f->f_zeiger)==f_sin)||((f->f_zeiger)==f_tan)||((f->f_zeiger)==f_cot) )
      {  fback=set_fkt_fkt(f->f_zeiger);
         (fback->LZ)=set_fkt_con(0);
         (fback->RZ)=kuerzen(f->RZ);
      };


  return(fback);
}

//_________________________LJAPUNOW_____________________________________


int lyapunov(unsigned char *sp,fknot* funct_a,fknot* ableit_a,fknot* funct_b,fknot* ableit_b,double* p,double a_start,double a_end,double b_start,double b_end,double startiter,unsigned int breite,unsigned int hoehe,int aiter,int iter)
{
 register int i,j,k,it1;
 register double ableitung,delta_a,delta_b;
 double index;
 delta_a=(a_end-a_start)/breite;
 delta_b=(b_end-b_start)/hoehe;

 p[1]=b_start;
 for (j=hoehe;j>0;j--)
 {p[0]=a_start;
  for(k=0;k<breite;k++)
  {
    p[23]=startiter;
    
    for (i=0;i<aiter;i++)
    { p[23]=(funct_a->f_zeiger(funct_a));
      p[23]=(funct_b->f_zeiger(funct_b));
    };

    index=0;it1=0;
    for (i=0;i<iter;i++)
    {
     p[23]=(funct_a->f_zeiger(funct_a));
     ableitung=(ableit_a->f_zeiger(ableit_a));
     if (ableitung!=0) {index=index+log(fabs(ableitung));it1++;}
    
     p[23]=(funct_b->f_zeiger(funct_b));
     ableitung=(ableit_b->f_zeiger(ableit_b));
     if (ableitung!=0) {index=index+log(fabs(ableitung));it1++;}
    }
    index = index/(double)(it1);
    memcpy(sp,&index,sizeof(double)); 
    sp+=sizeof(double);

    p[0]+=delta_a;
  }
  p[1]+=delta_b;
 }
 return(0);
}


