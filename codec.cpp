/*

 Convenience MP3 library.
 Enables easy PCM to MP3 conversion and vice versa.

 Copyright (C) 2015 Sergey Kolevatov

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.

 */

// $Revision: 1631 $ $Date:: 2015-03-24 #$ $Author: serge $

#include "codec.h"          // self

#include <fstream>          // std::ofstream
#include "../lameplus/lameplus.h"   // LamePlus
#include "../wave/wave.h"           // Wave

NAMESPACE_CONVIMP3_START

bool Codec::encode( const char *in_file, const char *out_file )
{
    Wave pcm( in_file );

    unsigned sample_rate    = pcm.get_samples_per_sec();
    unsigned byte_rate      = pcm.get_avg_bytes_per_sec();
    unsigned channels       = pcm.get_channels();

    std::ofstream mp3( out_file, std::ios_base::binary | std::ios_base::out  );

    const int PCM_SIZE = 8192;
    const int MP3_SIZE = 8192;

    unsigned offset = 0;

    std::vector<char> pcm_buffer;
    pcm_buffer.reserve( sizeof(short int) * PCM_SIZE * 2 );

    unsigned char mp3_buffer[MP3_SIZE];

    lameplus::LamePlus l;

    l.set_in_samplerate( sample_rate );
    l.set_brate( byte_rate );

    if( channels == 1 )
    {
        l.set_num_channels( 1 );
        l.set_mode( MONO );
    }
    else
    {
        l.set_num_channels( channels );
    }

    l.set_VBR( vbr_default );
    l.init_params();

    while( true )
    {
        int k = ( channels == 1 ) ? 1 : 2;

        unsigned size = PCM_SIZE * k * sizeof( short int );

        pcm.get_samples( offset, size, pcm_buffer );

        unsigned read = pcm_buffer.size();

        offset += read;

        if( read > 0 )
        {
            unsigned read_shorts = read / 2;  // need number of 'short int' read

            int write = 0;

            if( channels == 1 )
            {
                write = l.encode_buffer( reinterpret_cast<short int*>( &pcm_buffer[0] ), NULL, read_shorts, mp3_buffer, MP3_SIZE );
            }
            else
            {
                write = l.encode_buffer_interleaved( reinterpret_cast<short int*>( &pcm_buffer[0] ), read_shorts, mp3_buffer, MP3_SIZE );
            }

            pcm_buffer.clear();

            mp3.write( reinterpret_cast<char*>( mp3_buffer ) , write );
        }

        if( read < size )
        {
            int write = l.encode_flush( mp3_buffer, MP3_SIZE );

            mp3.write( reinterpret_cast<char*>( mp3_buffer ), write );

            break;
        }

    };

    return true;
}

bool Codec::decode( const char *in_file, const char *out_file )
{
    std::ifstream mp3( in_file, std::ios_base::binary | std::ios_base::in );

    if( mp3.is_open() == false )
    {
        return false;
    }

    const int PCM_SIZE = 8192;
    const int MP3_SIZE = 8192;

    short int pcm_buffer[PCM_SIZE * 2];
    unsigned char mp3_buffer[MP3_SIZE];

    lameplus::LamePlus l;

    l.set_decode_only( 1 );

    if( l.init_params() == -1 )
        return false;

//    mp3.read( reinterpret_cast<char*>( mp3_buffer ), k * sizeof(short int) * MP3_SIZE );
//
//    int read = mp3.gcount();

    return true;
}

NAMESPACE_CONVIMP3_END
