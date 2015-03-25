#include <cstdio>

#include "codec.h"  // convimp3::Codec

int main()
{
    printf( "Hello, world\n" );

    convimp3::Codec::encode( "test.wav", "test.mp3" );

    return 0;
}
