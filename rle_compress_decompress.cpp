/*
 * @Author: Ivan Chichvarin ichichvarin@humanplus.ru
 * @Date: 2024-04-06 21:21:52
 * @LastEditors: Ivan Chichvarin ichichvarin@humanplus.ru
 * @LastEditTime: 2024-04-06 21:22:03
 * @FilePath: /RLECompressor/rle_compress_decompress.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "compressor.h"
#include "decompressor.h"

#include <cassert>
#include <fstream>
#include <string>

using namespace std;

// эта функция нужна для теста
string GetFileContents(string file) {
    ifstream stream(file, ios::binary);

    stream.seekg(0, ios::end);
    size_t sz = stream.tellg();

    stream.seekg(0, ios::beg);
    string res(sz, '\0');

    if (stream.read(res.data(), sz)) {
        return res;
    }

    return {};
}

void Test() {
    ofstream("test.txt") << "abcdaaaaaabbbcdedecdec"s << string(1000, 'x')
                         << "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\xa0"
                            "decdecdecasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdassdasdassdas"
                            "dcdecasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdassdasdassdasdcde"
                            "casdasdasdasdasdasdasdasdasdasdasdasdasdasdasdassdasdassdasdcdecasd"
                            "asdasdasdasdasdasdasdasdasdasdasdasdasdasdassdasdassdasddeccccccccc"
                            "cccc"s;
    EncodeRLE("test.txt"s, "test.rle"s);
    DecodeRLE("test.rle"s, "test2.txt"s);

    assert(GetFileContents("test.rle"s).size() < GetFileContents("test.txt"s).size() / 2);
    assert(GetFileContents("test.txt"s) == GetFileContents("test2.txt"s));
}

int main(int argc, const char** argv) {
    if (argc == 1) {
        Test();
        return 0;
    }

    if (argc != 4 || (argv[1] != "x"s && argv[1] != "c"s)) {
        cout << "Usage: "s << argv[0] << " c|x <in file> <out file>"s << endl;
        return 1;
    }

    if (argv[1] == "c"s) {
        auto result = EncodeRLE(argv[2], argv[3]);
        if (!result.opened) {
            cout << "Error opening file"s << endl;
            return 2;
        }

        cout << "Compressed "s << result.src_size << " bytes => "s << result.dst_size << " bytes"s << endl;
    } else {
        if (!DecodeRLE(argv[2], argv[3])) {
            cout << "Error decoding file"s << endl;
            return 3;
        }

        cout << "Successfully decoded"s << endl;
    }

    return 0;
}