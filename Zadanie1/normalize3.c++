#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string>
#include <iostream>
#include <cctype>
#include <sstream>

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
    int write = 0;

    for (int read = 0; read < text.length(); read++) {
        if (text[read] != ' ' || (write > 0 && text[write-1] != ' ')) {
            text[write++] = text[read];
        }
    }

    text.resize(write);
}

std::string delete_duplicates(std::string& text) {
    std::stringstream st (text);
    std::string prev, curr, res, prev_clean, curr_clean;

    res.reserve(text.length());

    if (text.length() == 0) {
        return res;
    }

    if (st >> prev) {
        res.append(prev);
    } else {
        return text;
    }

    while (st >> curr) {
        prev_clean = prev;
        curr_clean = curr;

        if (!prev_clean.empty() && prev_clean.back() == ',') prev_clean.pop_back();
        if (!curr_clean.empty() && curr_clean.back() == ',') curr_clean.pop_back();

        if (prev_clean != curr_clean) res.append(" " + curr);
        else if (curr.back() == ',' && curr != prev) res += ','; 

        prev = curr;
    }
    return res;
}

void normalize(std::string& text) {
    int read, write;
    write = 0;

    for (int read=0;read<text.length();read++) {
        if (islower(text[read]) || isdigit(text[read])) {
            text[write++] = text[read];
        } else if (isspace(text[read])) {
            text[write++] = ' ';
        } else if (ispunct(text[read])) {
            text[write++] = ',';
        } else if (isupper(text[read])) {
            text[write++] = tolower(text[read]);
        }
    }
    text.resize(write);
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