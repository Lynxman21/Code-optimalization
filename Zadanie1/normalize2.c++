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

std::string remove_extra_spaces(std::string text) {
    int i = 0;
    std::string res;
    res.reserve(text.length());

    while (i < text.length()) {
        res += text[i];
        if (text[i] == ' ') {
            while (i < text.length() && text[i] == ' ') i++;
        } else {
            i++;
        }
    }

    return res;
}

std::string delete_duplicates(std::string text) {
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

std::string normalize(std::string text) {
    std::string res;
    res.reserve(text.length());
    int i;

    for (i=0;i<text.length();i++) {
        if (islower(text[i]) || isdigit(text[i])) {
            res += text[i];
        } else if (isspace(text[i])) {
            res += " ";
        } else if (ispunct(text[i])) {
            res += ",";
        } else if (isupper(text[i])) {
            res += tolower(text[i]);
        }
    }
    res = remove_extra_spaces(res);
    res = delete_duplicates(res);
    return res;
}

int main(int argc, char** argv) {
    std::string line, text, res;
    std::cout << "Start\n";

    double dtime;

    while (std::getline(std::cin, line)) {
        text.append(line + "\n");
    }
    dtime = dclock();
    for (int i=0; i<1000; i++) {
        res = normalize(text);
    }
    
    dtime = dclock() - dtime;

    std::cout << "Res: " << res << "\n";
    std::cout << "Time: " << dtime << "\n";

    fflush(stdout);
    return 0;
}