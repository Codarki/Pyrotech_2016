//.-----------------.
//| P y r o t e c h |-----------------------------------------------------------
//'-----------------'
//| 
//| Author:   Lasse Lopperi
//| Filename: sorter.h
//| Created:  6. joulukuuta 2004
//| 
//'---[ Versions ]--------------------------------------------------------------
//| 
//| v1.0 [6. joulukuuta 2004] : File created.
//| 
//'---------------------------------------------[ Copyright © Pyrotech, 2004 ]--

#pragma once
#ifndef PYROTECH_PT_SORTER_H
#define PYROTECH_PT_SORTER_H

namespace pt{

template<typename T>
void insertion_sort(T * input, int size, bool sort_ascending = true);

#include "sorter.inl"

} // namespace pt

#endif
