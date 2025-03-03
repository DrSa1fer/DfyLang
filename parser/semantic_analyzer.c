#include "include/analyzer.h"


static int analyze_declaration  (Analyzer_t *analyzer);

static int analyze_func_dec     (Analyzer_t *analyzer);
static int analyze_var_dec      (Analyzer_t *analyzer);
static int analyze_val_dec      (Analyzer_t *analyzer);

int analyze(Analyzer_t *analyzer) {

    return 0;
}


int analyze_val_dec      (Analyzer_t *analyzer) {
    if (analyzer->ast->type == NODE_DEC_VAL) {
        
    }
    return 0;
}