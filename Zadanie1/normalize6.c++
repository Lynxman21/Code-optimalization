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
#include <omp.h>

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

    int t = 4;
    char* results[4];
    size_t text_len = text.length();
    int starts[5];
    starts[0] = 0;
    starts[t] = text_len;

    for (int i=1;i<t;i++) {
        int pos = (text_len/t) * i;
        while (pos < text_len && text[pos] != ' ') pos++;
        if (pos < text_len) {
            starts[i] = pos+1;
        } else {
            starts[i] = text_len;
        }
    }

    char* res;
    dtime = dclock();

    for (int i=0;i<1000;i++) {
        #pragma omp parallel for num_threads(t)
        for (int i=0; i<t; i++) {
            int s = starts[i];
            int end = starts[i+1];

            if (s < end) {
                results[i] = normalize((char*)text.data() + s, end - s);
            } else {
                results[i] = (char*)calloc(1,1);
            }
        }

        size_t f_size = 0;
        for (int i=0; i<t; i++) {
            f_size += strlen(results[i]) + 1;
        }

        char* buff = (char*)malloc((f_size + 1) * sizeof(char));
        char* writer = buff;

        for (int i = 0; i<t; i++) {
            size_t len = strlen(results[i]);
            if (len > 0) {
                if (i > 0 && writer > buff) *(writer++) = ' ';
                memcpy(writer, results[i], len);
                writer += len;
            }
            free(results[i]);
        }

        *writer = '\0';
        res = delete_duplicates(buff, f_size);
        free(buff);

        if (i < 999) free(res);
    }
    
    dtime = dclock() - dtime;

    std::cout << "Res: " << res << "\n";
    std::cout << "Time: " << dtime << "\n";

    free(res);
    fflush(stdout);
    return 0;
}