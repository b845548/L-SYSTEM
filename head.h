
enum{
PARSE_READY,
PARSE_VARIABLE,
PARSE_AXIOM,
PARSE_RULES_SOURCE,
PARSE_RULES_REPLACE,
PARSE_ANGLE,
PARSE_ITERATION,
PARSE_DISTANCE,

};
#define MAX_RULES 10


struct lSystem{
char * resultat;
char * axiom;
char ** rules_replace;
char * rules_source;
int it_max;
int nb_rules;
float angle;
float pas;
};
typedef struct lSystem lSystem;

