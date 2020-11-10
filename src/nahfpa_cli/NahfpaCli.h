#ifndef NAHFPA_NAHFPACLI_H
#define NAHFPA_NAHFPACLI_H

typedef struct NahfpaCli NahfpaCli;

NahfpaCli* NahfpaCli_new(int argc, char *argv[]);
void NahfpaCli_free(NahfpaCli* self);

void NahfpaCli_execute(NahfpaCli* self);

#endif //NAHFPA_NAHFPACLI_H
