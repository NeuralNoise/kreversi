/* Yo Emacs, this -*- C++ -*-
 *******************************************************************
 *******************************************************************
 *
 *
 * KREVERSI
 *
 *
 *******************************************************************
 *
 * A Reversi (or sometimes called Othello) game
 *
 *******************************************************************
 *
 * created 1997 by Mario Weilguni <mweilguni@sime.com>
 *
 *******************************************************************
 *
 * This file is part of the KDE project "KREVERSI"
 *
 * KREVERSI is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * KREVERSI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KREVERSI; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *******************************************************************
 */


#include "qreversigame.h"


// ================================================================
//                      class QReversiGame


QReversiGame::QReversiGame(QObject *parent)
  : QObject(parent), Game()
{
}


QReversiGame::~QReversiGame()
{
}


void QReversiGame::newGame()
{
  Game::newGame();

  emit updateBoard();
  emit sig_score();
  emit turn(Game::toMove());
}


bool QReversiGame::makeMove(Move move)
{
  bool  retval = Game::makeMove(move);
  if (!retval)
      return false;

  emit sig_move(m_moveNumber, move);
  emit updateBoard();
  emit sig_score();
  emit turn(Game::toMove());

  if (!Game::moveIsAtAllPossible())
    emit gameOver();

  return retval;
}


bool QReversiGame::takeBackMove()
{
  bool  retval = Game::takeBackMove();

  // Update all views.
  emit updateBoard();
  emit sig_score();
  emit turn(Game::toMove());

  return retval;
}


#include "qreversigame.moc"

