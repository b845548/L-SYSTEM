#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"head.h"

static void recursive(lSystem * ls,int it_actuel){
int i,j,len=strlen(ls->resultat);
int k,it,new_len;
char * nv;
if(ls->it_max==it_actuel)return;

for(new_len=0,i=0;i<len;i++){
	for(j=0;j<ls->nb_rules;j++)
		if(ls->resultat[i]==ls->rules_source[j])
			break;
						
	if(j<ls->nb_rules){
		new_len+=strlen(ls->rules_replace[j]);
	}else{
		new_len++;
	}				
}
nv=malloc(sizeof(*nv)*new_len+1);
//printf("%d ",new_len);

for(i=0,it=0;i<len;i++){
	for(j=0;j<ls->nb_rules;j++)
		if(ls->resultat[i]==ls->rules_source[j])
			break;
						
	if(j<ls->nb_rules){
		new_len=strlen(ls->rules_replace[j]);
		for(k=0;k<new_len;k++)		
			nv[it++]=ls->rules_replace[j][k];
	}else{
		nv[it++]=ls->resultat[i];
	}				
}

nv[it]='\0';
//printf("%s ",ls->resultat);

free(ls->resultat);
ls->resultat=nv;
recursive(ls,1+it_actuel);
}




lSystem parsing_lsystem(char * path){
lSystem ls;
FILE * f;
char * pch;
int state=PARSE_READY;
char buf[100];
char buf_variable[100];
int nb_varible=0;
char source_char=0;
int i;
f=fopen(path,"r");
ls.rules_replace=malloc(sizeof(char*)*MAX_RULES);
ls.nb_rules=0;

while(NULL!=fgets(buf,100,f)){
  if(buf[0]=='#')
	continue;
  pch = strtok (buf," :\t");
  while (pch != NULL)
  {
	switch(state){
		case PARSE_ITERATION:
			if(strchr(pch,'\n')!=NULL)
				state=PARSE_READY;
			ls.it_max=atoi(pch);
		break;
		case PARSE_DISTANCE:
			if(strchr(pch,'\n')!=NULL)
				state=PARSE_READY;
			ls.pas=atof(pch);
		break;
		case PARSE_ANGLE:
			if(strchr(pch,'\n')!=NULL)
				state=PARSE_READY;
			ls.angle=atof(pch);
		break;
		case PARSE_RULES_SOURCE:
		case PARSE_RULES_REPLACE:
			if(strchr(pch,'\n')!=NULL){
				if(pch[strlen(pch)-1]=='\n')
				for(i=0;i<MAX_RULES;i++)
					if(source_char==ls.rules_source[i]){			 
						ls.rules_replace[i]=malloc(sizeof(char)*strlen(pch));
						strcpy(ls.rules_replace[i],pch);
						ls.rules_replace[i][strlen(pch)-1]='\0';	
						ls.nb_rules++;
						break;				
					}
			
				state=PARSE_READY;
			}else if(strchr(pch,',')!=NULL)
				state=PARSE_RULES_SOURCE;
			else if(state==PARSE_RULES_SOURCE&&strchr(pch,'=')!=NULL)			
				state=PARSE_RULES_REPLACE;
			else if(state==PARSE_RULES_SOURCE){

				source_char=*pch;
			printf ("%c\n",*pch);
			}else if(state==PARSE_RULES_REPLACE){
			for(i=0;i<MAX_RULES;i++)
					if(source_char==ls.rules_source[i]){			 
						ls.rules_replace[i]=malloc(sizeof(char)*strlen(pch));
						strcpy(ls.rules_replace[i],pch);
						ls.rules_replace[i][strlen(pch)-1]='\0';
						
						ls.nb_rules++;
						break;				
					}
			}
		// printf ("%s\n",pch);


		break;
		case PARSE_AXIOM:
			if(strchr(pch,'\n')!=NULL)
				state=PARSE_READY;
			ls.axiom=malloc(strlen(pch)*sizeof(char));
			strcpy(ls.axiom,pch);
		break;
		case PARSE_VARIABLE:
			if(strchr(pch,'\n')!=NULL){
				if(*pch!='\n')
					buf_variable[nb_varible++]=*pch;		    
				buf_variable[nb_varible++]='\0';
				ls.rules_source=malloc(nb_varible*sizeof(char));
				strcpy(ls.rules_source,buf_variable);
				state=PARSE_READY;
	//			printf ("varialbe : %s",buf_variable);
	//			printf ("varialbe : %s",ls.rules_source);
			break;
			}
			if(strchr(pch,',')!=NULL)
				break;
		buf_variable[nb_varible++]=*pch;		    
//		printf ("arialbe : %s",pch);
	
		break;
		default:
		break;
	}
	if(strcmp(pch,"variables")==0){
		state=PARSE_VARIABLE;
		nb_varible=0;
	}else if(strcmp(pch,"rules")==0)
		state=PARSE_RULES_SOURCE;
	else if(strcmp(pch,"axiom")==0)
		state=PARSE_AXIOM;
	else if(strcmp(pch,"angle")==0)
		state=PARSE_ANGLE;
	else if(strcmp(pch,"distance")==0)
		state=PARSE_DISTANCE;
	else if(strcmp(pch,"iteration")==0)
		state=PARSE_ITERATION;
	pch = strtok (NULL, " :\t");
	

	
  }
}
	fclose(f);
			 for(i=0;i<ls.nb_rules;i++)
			    printf ("%d rules : %s\n",i,ls.rules_replace[i]);
		    printf ("angle : %f\n",ls.angle);
		    printf ("distance : %f\n",ls.pas);
		    printf ("variable : %s\n",ls.rules_source);
		    printf ("axiom : %s",ls.axiom);
		    printf ("iteration : %d",ls.it_max);
	ls.resultat=(char *)malloc(strlen(ls.axiom)*sizeof(char));
	strcpy(ls.resultat,ls.axiom);
	recursive(&ls,0);
 return ls;

}


