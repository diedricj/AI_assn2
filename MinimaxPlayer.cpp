/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */
#include <iostream>
#include <assert.h>
#include <algorithm>
#include "MinimaxPlayer.h"

using std::vector;

MinimaxPlayer::MinimaxPlayer(char symb) :
    Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}

void MinimaxPlayer::get_successors(OthelloBoard* b, OthelloBoard** successors, int*cols, int* rows, int& count, char symbol) {
    int row, col = 0;
    for (row = 0; row < b->get_num_rows(); row++) {
        for (col = 0; col < b->get_num_cols(); col++) {
            if (b->is_legal_move(col, row, symbol)) {
                successors[count] = new OthelloBoard(*(b));
                successors[count]->play_move(col, row, symbol);
                cols[count] = col;
                rows[count] = row;
                count++;
            }
        }
    }
}

bool MinimaxPlayer::is_terminal_state(OthelloBoard* b) {
    if ((b->has_legal_moves_remaining(b->get_p1_symbol())) || b->has_legal_moves_remaining(b->get_p2_symbol())) {
        return false;
    }
    else {
        return true;
    }
}

int MinimaxPlayer::utility(OthelloBoard* b) {
    int val = b->count_score(b->get_p1_symbol()) - b->count_score(b->get_p2_symbol());
    return val;
}

int MinimaxPlayer::max_value(OthelloBoard* b) {
    if (is_terminal_state(b)) {
        return utility(b);
    }

    int val = -17;
    int count = 0;
    int cols[16];
    int rows[16];

    OthelloBoard** successors = new OthelloBoard * [16];
    char symbol = b->get_p1_symbol();
    get_successors(b, successors, cols, rows, count, symbol);
    for (int i = 0; i < count; i++) {
        val = std::max(val, min_value(successors[i]));
    }



    for (int i = 0; i < count; i++)
        delete successors[i];

    return val;
}


int MinimaxPlayer::min_value(OthelloBoard* b) {
    if (is_terminal_state(b)) {
        return utility(b);
    }

    int val = 17;
    int count = 0;
    int cols[16];
    int rows[16];

    OthelloBoard** successors = new OthelloBoard * [16];
    char symbol = b->get_p2_symbol();
    get_successors(b, successors, cols, rows, count, symbol);


    for (int i = 0; i < count; i++) {
        val = std::min(val, max_value(successors[i]));
    }

    for (int i = 0; i < count; i++)
        delete successors[i];

    return val;
}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {

    OthelloBoard** successors = new OthelloBoard * [16];
    int cols[16];
    int rows[16];
    int count = 0;

    get_successors(b, successors, cols, rows, count, get_symbol());

    int choice = 0;
    int temp;
    int min = 100;
    int max = -100;

    if (get_symbol() == b->get_p2_symbol()) {
        for (int i = 0; i < count; i++) {
            temp = max_value(successors[i]);
            if (min > temp) {
                min = temp;
                choice = i;
            }
        }
    }
    else {
        for (int i = 0; i < count; i++) {
            temp = min_value(successors[i]);
            if (max < temp) {
                max = temp;
                choice = i;
            }
        }
    }

    col = cols[choice];
    row = rows[choice];

}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
