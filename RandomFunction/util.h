#pragma once

typedef unsigned char byte;

byte getBit(byte b, byte pos);

byte mirrorByte(byte b);

byte slideByte(byte b, byte pos);

byte pullLeftByte(byte b);

void printb(byte b);