#pragma once
void init_variables(int _dimension, int _episode, int _interval, int _groups, double _alpha, double _gamma);

void training_model_cost(int init_state);

void delete_heap_memory();
