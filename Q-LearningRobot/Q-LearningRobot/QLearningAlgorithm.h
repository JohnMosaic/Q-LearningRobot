#pragma once
void init_variables(int _matrix_dimension, int _max_episode, double _alpha, double _gamma);

void training_model(int init_state, bool is_print_process);

int inference_best_action(int now_state);

void delete_heap_memory();
