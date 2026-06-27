#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string>
#include <iostream>
#include <cctype>
#include <sstream>
#include <algorithm>

static double gtod_ref_time_sec = 0.0;

double dclock()
{
    double the_time, norm_sec;
    struct timeval tv;
    gettimeofday( &tv, NULL );
    if ( gtod_ref_time_sec == 0.0 )
        gtod_ref_time_sec = ( double ) tv.tv_sec;
    norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;
    the_time = norm_sec + tv.tv_usec * 1.0e-6;
    return the_time;
}

// zostały duplikaty oraz małe litery

void remove_extra_spaces(std::string& text) {
    auto res = std::unique(text.begin(), text.end(), [](char a, char b) {
        return a == b && a == ' ';
    });

    text.erase(res, text.end());
}

std::string delete_duplicates(std::string& text) {
    auto begin_prev = text.begin();
    auto end_prev = std::find(text.begin(), text.end(), ' ');
    auto begin_next = std::find_if(end_prev, text.end(), [](char c) {
        return c != ' ';
    });
    auto end_next = std::find(begin_next, text.end(), ' ');

    std::string res;
    res.reserve(text.length());
    res.append(begin_prev, end_prev);

    while (begin_next != text.end()) {
        auto temp_end_prev = end_prev;
        auto temp_end_next = end_next;

        if (begin_prev != temp_end_prev && *(temp_end_prev - 1) == ',') --temp_end_prev;
        if (begin_next != temp_end_next && *(temp_end_next - 1) == ',') --temp_end_next;
        
        if (!std::equal(begin_prev, temp_end_prev, begin_next, temp_end_next)) {
            res += ' ';
            res.append(begin_next, end_next);
            begin_prev = begin_next;
            end_prev = end_next;
        }

        begin_next = std::find_if(end_next, text.end(), [](char c) {
            return c != ' ';
        });
        end_next = std::find(begin_next, text.end(), ' ');
    }

    return res;
}

void normalize(std::string& text) {
    std::transform(text.begin(), text.end(), text.begin(), [](char c) {
            if (islower(c) || isdigit(c)) {
                return c;
            } else if (isspace(c)) {
                return ' ';
            } else if (ispunct(c)) {
                return ',';
            } else if (isupper(c)) {
                return (char)std::tolower(c);
            }
            return '#';
    });

    auto e = std::remove_if(text.begin(), text.end(), [](char c) {
            return c == '#';
    });

    text.erase(e, text.end());
    remove_extra_spaces(text);
    text = delete_duplicates(text);
}

int main(int argc, char** argv) {
    std::string line, text;
    std::cout << "Start\n";

    double dtime;

    while (std::getline(std::cin, line)) {
        text.append(line + "\n");
    }
    dtime = dclock();
    for (int i=0; i<1000; i++) {
        std::string temp = text;
        normalize(temp);
        if (i == 999) text = temp;
    }
    dtime = dclock() - dtime;

    std::cout << "Res: " << text << "\n";
    std::cout << "Time: " << dtime << "\n";

    fflush(stdout);
    return 0;
}