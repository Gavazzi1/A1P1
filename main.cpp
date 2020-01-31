#include "cast_helper.h"
#include "hashmap.h"
#include "helper1.h"
#include "helper2.h"
#include "helper3.h"
#include "integer.h"
#include "list.h"
#include "object.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1 is String, 2 is Float, 3 is Int, 4 is Bool, 0 is missing value
void print_without_schema(Object *object) {
  String *temp = cast_string(object);
  assert(temp);
  size_t size_ = temp->get_size();
  const char *value_ = temp->getValue();
  int type = atoi(identify_datatype(value_));
  char buf[size_];
  // printf("%d\n", type);
  if (value_[0] == '<' && value_[size_ - 1] == '>') {
    for (size_t i = 1; i < size_ - 1; i++) {
      buf[i - 1] = value_[i];
    }
    buf[size_ - 2] = 0;
  }
  if (type == 1) {
    if (buf[0] == '"') {
      printf("%s", buf);
    } else {
      printf("\"%s\"", buf);
    }
  } else {
    printf("%s", buf);
  }
}

int main(int argv, char **argc) {
  if (argv == 1) {
    printf("Usage: %s -key argument\n", argc[0]);
    exit(0);
  }
  Hashmap *command_map = new Hashmap();
  Hashmap *data_map = new Hashmap();
  read_command(command_map, argv, argc);
  int max_row = read_file(command_map, data_map);

  StrList *header_type = get_column_header(max_row, data_map);
  String *print_col_type_idx = cast_string(command_map->get(print_col_type));
  if (print_col_type_idx) {
    size_t col = atoi(print_col_type_idx->getValue());
    size_t type = atoi(header_type->get(col)->getValue());
    // header_type->print_self();
    // printf("\n");
    if (type == 1) {
      std::cout << "STRING" << std::endl;
    } else if (type == 2) {
      std::cout << "FLOAT" << std::endl;
    } else if (type == 3) {
      std::cout << "INT" << std::endl;
    } else if (type == 4) {
      std::cout << "BOOL" << std::endl;
    } else if (type == 5) {
      std::cout << "MISSING VALUE" << std::endl;
    }
  }

  // print out -is_missing_idx
  StrList *is_missing_idx_idx = cast_list(command_map->get(is_missing_idx));
  if (is_missing_idx_idx) {
    String *column = is_missing_idx_idx->get(0);
    String *offset = is_missing_idx_idx->get(1);
    Integer *col = new Integer(atoi(column->getValue()));
    Integer *off = new Integer(atoi(offset->getValue()));
    StrList *list = cast_list(data_map->get(off));
    if (list == nullptr) {
      std::cout << 1 << std::endl;
    } else {
      if (col->val_ >= list->size()) {
        std::cout << 1 << std::endl;
      } else {
        if (strcmp(list->get(col->val_)->getValue(), "<>") != 0) {
          std::cout << 0 << std::endl;
        } else {
          std::cout << 1 << std::endl;
        }
      }
    }
  }

  // print out -print_col_idx
  StrList *idx = cast_list(command_map->get(print_col_idx));
  if (idx) {
    String *column = idx->get(0);
    String *offset = idx->get(1);
    Integer *col = new Integer(atoi(column->getValue()));
    Integer *off = new Integer(atoi(offset->getValue()));
    StrList *list = cast_list(data_map->get(off));
    Object *temp = list->get(col->val_);
    print_without_schema(temp);
    printf("\n");
  }

  // print out

  return 0;
}
