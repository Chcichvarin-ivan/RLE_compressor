/*
 * @Author: Ivan Chichvarin ichichvarin@humanplus.ru
 * @Date: 2024-04-06 21:20:59
 * @LastEditors: Ivan Chichvarin ichichvarin@humanplus.ru
 * @LastEditTime: 2024-04-07 16:55:13
 * @FilePath: /RLECompressor/decompressor.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <string>
#include <fstream>


#define MAX_BLOCK_SIZE 128

inline bool DecodeRLE(const std::string& src_name, const std::string& dst_name){
    using namespace std;

    ifstream in(src_name, std::ios::in | std::ios::binary);
    if (!in) {
        return false;
    }
    ofstream out(dst_name, std::ios::out | std::ios::binary);

    do{
        unsigned char header = in.get();
        bool is_series = (header & 1);
        size_t data_block_length = static_cast<size_t>(header >> 1) + 1;
        
        if(!in.eof()){
            if(!is_series){
                char block_buffer[MAX_BLOCK_SIZE];
                in.read(block_buffer,data_block_length);
                size_t bytes_been_read = in.gcount();
                out.write(block_buffer, bytes_been_read);
            }else{
                unsigned char abbreviated_character = in.get();
                string restored_block(data_block_length,abbreviated_character);
                out.write(restored_block.data(), data_block_length);
            }

        }//if(!in.eof())
    }while(in);
    return true;
}