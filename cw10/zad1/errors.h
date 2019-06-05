#pragma once
void pdie(char* message, int exit_code);
void die(char* message, int exit_code);

int perr(char* message, int return_code);
int err(char* message, int return_code);

void outerr(char* message);
void outperr(char* message);