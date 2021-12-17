// Pawn class publicly inherits the Piece class which has all the base attributes that chess pieces should contain
// The Pawn class has its own member functions that only apply to all Pawn pieces on the board
// It has member functions that override the pure virtual functions in the Piece class.

#ifndef INC_4PCHESS_PAWN_H
#define INC_4PCHESS_PAWN_H

#include "Piece.h"

class Pawn : public Piece
{

private:
    bool is_2_move_opening = false;

public:
    Pawn(Player &player, Game &game, int x, int y);

    virtual Piece::Type get_type() const override;

    virtual int get_point() const override;

    virtual int list_possible_moves() override;

    std::string get_img() const override;
};

#endif //INC_4PCHESS_PAWN_H
