#ifndef COMMAND_H__
#define COMMAND_H__

#include <stdio.h>
#include <stdlib.h>

#include "task.h"
#include "Array.h"
#include "BST.h"

status_codes loadProcessing(Node **root, const char *filename, const char array_name);

status_codes saveProcessing(Node *root, const char *filename, const char array_name);

status_codes randProcessing(Node **root, const char array_name, int count, int left_bound, int right_bound);

status_codes concatProcessing(Node **root, const char dest_name, const char src_name);

status_codes freeProcessing(Node *root, const char array_name);

status_codes removeProcessing(Node *root, const char array_name, const int start, const int count);

status_codes copyProcessing(Node **root, const char src_name, const char dest_name, const int start, const int end);

status_codes sortProcessing(Node *root, const char array_name, int sort_flag);

status_codes statsProcessing(Node *root, const char array_name);

status_codes printProcessing(Node *root, const char array_name, const int start, const int end);

#endif