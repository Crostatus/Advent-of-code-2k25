#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_LINE_LEN 4096

int count_digits(long long n) {
    if (n == 0)
        return 1;

    if (n < 0)
        n = -n;

    return (int)floor(log10((long double)n)) + 1;
}

long long min_number_with_digits(int digits) {
    if (digits <= 0)
        return 0;
    return (long long)pow(10, digits - 1);
}

long long max_number_with_digits(int digits) {
    if (digits <= 0)
        return 0;
    return (long long)pow(10, digits) - 1;
}

long long maximize_start(long long start) {
    int digits = count_digits(start);

    // always even digits amount
    int half = digits / 2;
    
    char str_start[digits + 1];
    sprintf(str_start, "%lld", start);

    // build <a1, a2>
    char a1[half + 1];
    char a2[half + 1];

    strncpy(a1, str_start, half);
    a1[half] = '\0';

    strncpy(a2, str_start + half, half);
    a2[half] = '\0';

    // convert back to int
    long long n1 = atoll(a1);
    long long n2 = atoll(a2);
    
    long long mx = (n1 > n2 ? n1 : n2);

    // create final string <a1, max(a1, a2)>
    char result_str[digits + 1];
    snprintf(result_str, sizeof(result_str), "%s%0*lld", a1, half, mx);    

    return atoll(result_str);
}

long long minimize_end(long long end) {
    int digits = count_digits(end);

    // always even digits amount
    int half = digits / 2;
    
    char str_end[digits + 1];
    sprintf(str_end, "%lld", end);

    // build <b1, b2>
    char b1[half + 1];
    char b2[half + 1];

    strncpy(b1, str_end, half);
    b1[half] = '\0';

    strncpy(b2, str_end + half, half);
    b2[half] = '\0';

    // convert back to int
    long long n1 = atoll(b1);
    long long n2 = atoll(b2);
    
    long long mn = (n1 < n2 ? n1 : n2);

    // create final string <b1, min(b1, b2)>
    char result_str[digits + 1];
    snprintf(result_str, sizeof(result_str), "%s%0*lld", b1, half, mn);    

    return atoll(result_str);
}


bool is_mirrored(long long n) {
    char buf[32];
    sprintf(buf, "%lld", n);
    
    int len = strlen(buf);
    int half = len / 2;

    // copy first half
    char left[32];
    memcpy(left, buf, half);
    left[half] = '\0';

    // copy second half
    char right[32];
    memcpy(right, buf + half, half);
    right[half] = '\0';

    bool equal = (strcmp(left, right) == 0);

    char mirrored_str[32];
    memcpy(mirrored_str, left, half);
    memcpy(mirrored_str + half, left, half);
    mirrored_str[len] = '\0';    

    return equal;
}


long long find_and_sum(long long start, long long end) {
    int start_digit_count = count_digits(start);
    int end_digit_count = count_digits(end);

    if(start_digit_count % 2 != 0) {
        start = min_number_with_digits(start_digit_count + 1);
    }

    if(end_digit_count % 2 != 0) {
        end = max_number_with_digits(end_digit_count - 1);
    }

    if(start > end) {
        return 0;
    }

    start = maximize_start(start);
    end = minimize_end(end);

    if(start > end) {
        return 0;
    }

    long long mirror_sum = 0;
    long long curr = start;   
    while(curr <= end) {
        if(count_digits(curr) % 2 != 0) {            
            continue;
        }        
        if(is_mirrored(curr)) {
            printf("Found mirrored number: %lld\n", curr);
            mirror_sum += curr;
        }
        curr++;
    }

    return mirror_sum;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Use: %s <file_name>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    char buffer[MAX_LINE_LEN];
    long long password = 0;
    while (fgets(buffer, sizeof(buffer), fp)) {
        // Remove final newline
        buffer[strcspn(buffer, "\n")] = '\0';
        
        char *token = strtok(buffer, ",");
        while (token != NULL) {
            
            char *dash = strchr(token, '-');
            if (!dash) {
                fprintf(stderr, "Invalid string token: %s\n", token);
                fclose(fp);
                return 1;            
            } 

            *dash = '\0';
            long long start = atoll(token);
            long long end   = atoll(dash + 1);

            password += find_and_sum(start, end);

            token = strtok(NULL, ",");
        }
    }

    printf("Password: %lld\n", password);

    fclose(fp);
    return 0;
}
