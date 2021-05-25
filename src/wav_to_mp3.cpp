// wav_to_mp3.cpp : Defines the entry point for the application.
//

#include "wav_to_mp3.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "Using LAME version " << get_lame_version() << '\n';
    auto usage = [](const char* name) {
        cout << "Usage: " << name << " <input/output directory>" << '\n';
    };
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }
    const string path(argv[1]);
    vector<string> files;

    try {
        files = readDirectory(path, ".wav");
    }
    catch (const exception & e) {
        cout << "Error reading from " << path << ": " << e.what() << '\n';
        usage(argv[0]);
        return 1;
    }
    if (files.size()) {
        {
            for (auto file_name : files)
            {
                std::cout << file_name << "\n";
            }
        }
        try {
            ThreadPool obj = ThreadPool(files, LameEncode);
        }
        catch (std::runtime_error & e) {
            cout << e.what();
            exit(1);
        }
    }
    else {
        cout << "Error: no suitable WAV files found in " << path << '\n';
        usage(argv[0]);
        return 1;
    }
}


/**
 * Read a directory listing into a vector of strings, filtered by file extension.
 * Throws std::exception on error.
 **/
static vector<string> readDirectory(const string& directoryLocation, const string& extension)
{
    vector<string> result;

    auto strToLower = [](const string& input) {
        string result(input.length(), 0);
        transform(input.begin(), input.end(), result.begin(), ::tolower);
        return result;
    };
    auto GetFileSize = [](const string& filename) {
        struct stat stat_buf;
        int rc = stat(filename.c_str(), &stat_buf);
        return rc == 0 ? stat_buf.st_size : -1;
    };
    string lcExtension(strToLower(extension));

    auto* dir = opendir(directoryLocation.c_str());
    if (!dir)
        throw std::runtime_error("Unable to open directory.");

    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr)
    {
        string entry(ent->d_name);
        string lcEntry(strToLower(entry));

        // Check extension matches (case insensitive)
        auto pos = lcEntry.rfind(lcExtension);
        if (pos != string::npos && pos == lcEntry.length() - lcExtension.length()) {
            auto path = directoryLocation + '/' + entry;
            if (GetFileSize(path) > 0)
                result.push_back(path);
            else
                cout << "Truncated wav file: " << path << '\n';
        }
    }

    if (closedir(dir) != 0)
        throw std::runtime_error("Unable to close directory.");
    ent = nullptr;
    dir = nullptr;
    return result;
}

bool LameEncode(const string& input)
{
    
    const string ext = { "mp3" };

    string output(input);
    output.replace(output.end() - ext.length(), output.end(), ext);

    FILE* wav = fopen(input.c_str(), "rb");
    FILE* mp3 = fopen(output.c_str(), "wb");

    int read, write;
    const size_t IN_SAMPLERATE = 44100; // default sample-rate
    const int WAV_SIZE = 8192;
    const int MP3_SIZE = 8192;
    const size_t LAME_GOOD = 5;
    std::vector<short int> pcm_buffer(WAV_SIZE * 2);    
    std::vector<unsigned char> mp3_buffer(MP3_SIZE);
    const size_t bytes_per_sample = 2 * sizeof(int16_t); // stereo signal, 16 bits

    lame_t lame = lame_init();
    lame_set_in_samplerate(lame, IN_SAMPLERATE);
    lame_set_VBR(lame, vbr_default);
    lame_set_VBR_q(lame, LAME_GOOD);
    lame_init_params(lame);

    do {
        read = fread(pcm_buffer.data(), bytes_per_sample, WAV_SIZE, wav);
        if (read == 0)
        {
            write = lame_encode_flush(lame, mp3_buffer.data(), MP3_SIZE);
        }
        else
        {
            write = lame_encode_buffer_interleaved(lame, pcm_buffer.data(), read, mp3_buffer.data(), MP3_SIZE);
        }
        fwrite(mp3_buffer.data(), write, 1, mp3);
    } while (read != 0);

    lame_close(lame);
    fclose(mp3);
    fclose(wav);

    return true;
}