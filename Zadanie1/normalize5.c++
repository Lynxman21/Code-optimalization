#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string>
#include <iostream>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <cstring>

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

char* find_end(char*start) {
    char* c = start;

    while (*c != ' ' && *c != '\0') {
        c++;
    }

    return c;
}

char* delete_duplicates(char* text, size_t size) {
    char* begin_prev = text;
    char* end_prev = find_end(begin_prev);

    char* res = (char*)malloc(sizeof(char) * (size + 1));
    char* writer = res;
    int len = end_prev - begin_prev;
    memcpy(res, begin_prev, len);
    writer += len;

    if (*end_prev == '\0') {
        *writer = '\0';
        return res;
    }

    auto begin_next = end_prev + 1;
    auto end_next = find_end(begin_next);

    while (*begin_next != '\0') {
        auto temp_end_prev = end_prev;
        auto temp_end_next = end_next;

        if (begin_prev != temp_end_prev && *(temp_end_prev - 1) == ',') --temp_end_prev;
        if (begin_next != temp_end_next && *(temp_end_next - 1) == ',') --temp_end_next;

        len = temp_end_prev - begin_prev;
        size_t len_next = temp_end_next - begin_next;
        
        if (len != len_next || memcmp(begin_prev, begin_next, len_next) != 0) {
            *(writer++) = ' ';
            memcpy(writer, begin_next, end_next - begin_next);
            writer += end_next - begin_next;

            begin_prev = begin_next;
            end_prev = end_next;
        }

        begin_next = end_next;
        if (*begin_next != '\0') begin_next++;
        end_next = find_end(begin_next);
    }

    *writer = '\0';
    return res;
}

char* normalize(char* text, size_t size) {
    int write = 0;
    int interval = int('a') - int('A');
    char* temp = (char*)malloc(sizeof(char) * (size + 1));

    for (int i=0;i<size;i++) {
        if (std::islower(text[i]) || std::isdigit(text[i])) {
            temp[write++] = text[i];
        } else if (std::isspace(text[i]) && write > 0 && temp[write-1] != ' ') {
            temp[write++] = ' ';
        } else if (std::isupper(text[i])) {
            temp[write++] = (char)(text[i] + interval);
        } else if (std::ispunct(text[i]) && write > 0 && temp[write-1] != ',') {
            temp[write++] = ',';
        }
    }

    temp[write] = '\0';
    char* result = delete_duplicates(temp, write);
    free(temp);
    return result;
}

int main(int argc, char** argv) {
    std::string line, text;
    std::cout << "Start\n";
    double dtime;

    while (std::getline(std::cin, line)) {
        text.append(line + "\n");
    }

    char* result;

    dtime = dclock();
    for (int i=0;i<1000;i++) {
        result = normalize((char*)text.data(), text.length());

        if (i < 999) free(result);
    }
    
    dtime = dclock() - dtime;

    std::cout << "Res: " << result << "\n";
    std::cout << "Time: " << dtime << "\n";

    free(result);
    fflush(stdout);
    return 0;
}