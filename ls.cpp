#include <iostream>
#include <vector>
#include <algorithm>

#include <dirent.h>
#include <error.h>

#include "ls.h"

using namespace std;

vector<string> fnames_in_dir(const char* path)
{
    vector<string> v;

    errno = 0;
    // open directory specified by path
    if (DIR* d = opendir(path))
    {
        struct dirent *dir = NULL;
        errno = 0;
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_name[0] != '.')
                v.push_back(dir->d_name);
        }

        if (errno != 0)
            perror("readdir");

        closedir(d);
    }
    else
    {
        // NULL returned, error!
        perror("opendir");
    }

    auto make_lower = [](string a, string b)
    {
        for (auto& c : a)
            c = tolower(c);
        for (auto& c : b)
            c = tolower(c);

        return a < b;
    };

    sort(v.begin(), v.end(), make_lower);

    return v;
}

/*
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << argv[0] << " [path]\n";
        return 1;
    }

    vector<string> fnames = fnames_in_dir(argv[1]);

    for (const auto& f : fnames)
        cout << f << '\n';

    return 0;
}
*/
