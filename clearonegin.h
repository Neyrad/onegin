const int ru_A   = -64;
const int ru_ya  = - 1;

const int ru_YO  = -88;
const int ru_yo  = -72;

const int ru_YE  = -59;
const int ru_ZHE = -58;

const int ru_ye  = -27;
const int ru_zhe = -26;

typedef struct {
    char* start;
    char* end;
}line;

int my_strcmp(const void* A, const void* B);
void swap(line* array, int i, int j);
int partition(line* array, int start, int end, int (*compar) (const void*, const void*), int nLines);
void ney_qsort(line* array, int start, int end, int (*compar) (const void*, const void*), int nLines); 
int noLetters(char* string);
int rhyme_strcmp(const void* A, const void* B);
int my_compare(int symbA, int symbB);
int isLetter(char X);

const int COMPAR_ERROR = 888;
const int tolerance = 1e-6;

int my_strcmp (const void* A, const void* B) {
    char* strA = ((line*)A)->start;
    char* strB = ((line*)B)->start;

	int counter = 0;
	int shiftA  = 0;
	int shiftB  = 0;

	for (shiftA = 0; !isLetter (*(strA + shiftA))
	       		  && *(strA + shiftA) != '\0'; ++shiftA);
	for (shiftB = 0; !isLetter (*(strB + shiftB))
                  && *(strB + shiftB) != '\0'; ++shiftB);

	for (counter = 0; *(strA + shiftA + counter) != '\0' 
		 	 	   && *(strB + shiftB + counter) != '\0'; ++counter) {
    	if      (my_compare(*(strA + shiftA + counter), *(strB + shiftB + counter)) == 2)
			return -1;
		else if (my_compare(*(strA + shiftA + counter), *(strB + shiftB + counter)) == 1)
			return  1;
	}

	if      (*(strA + shiftA + counter) == '\0'
		  && *(strB + shiftB + counter) == '\0')
		return  0;
	else if (*(strA + shiftA + counter) == '\0')
		return -1;
	else if (*(strB + shiftB + counter) == '\0')
		return  1;
	else
		return COMPAR_ERROR;
}

int partition(line* array, int start, int end, int (*compar) (const void*, const void*), int nLines) {
    line pivot = array [end];
    int pindex = start;

    for (int i = start; i < end; ++i) {
        if (compar (&array [i], &pivot) <= 0) {
            swap (array, i, pindex++);
        }
    }
    swap (array, end, pindex);
    return pindex;
}

void swap(line* array, int i, int j) {
    if (i == j)
        return;
    line temp = array [i];
    array [i] = array [j];
    array [j] = temp;
}

void ney_qsort(line* array, int start, int end, int (*compar) (const void*, const void*), int nLines) {
    if (start < end) {
        int pindex = partition (array, start, end, compar, nLines);
        ney_qsort (array, start, pindex - 1, compar, nLines);
        ney_qsort (array, pindex + 1, end, compar, nLines);
    }
}

int noLetters(char* string) {
	int counter = 0, not_letters = 0;
	for (counter = 0; *(string + counter) != '\0'; ++counter)
		if (!isLetter(*(string + counter)))
			++not_letters;
	return not_letters == counter;
}

int rhyme_strcmp(const void* A, const void* B) {
    char* strA = ((line*)A)->end;
    char* strB = ((line*)B)->end;

	int counter = 0;
	int shiftA  = 0;
	int shiftB  = 0;
    
	for (shiftA = 0; !isLetter (*(strA - shiftA))
	              && *(strA - shiftA) != '\0'; ++shiftA);
	for (shiftB = 0; !isLetter (*(strB - shiftB))
		          && *(strB - shiftB) != '\0'; ++shiftB);

	for (counter = 0; *(strA - shiftA - counter) != '\0' 
		 	 	   && *(strB - shiftB - counter) != '\0'; ++counter) {
        if      (my_compare(*(strA - shiftA - counter), *(strB - shiftB - counter)) == 2)
			return -1;
		else if (my_compare(*(strA - shiftA - counter), *(strB - shiftB - counter)) == 1)
			return  1; 
	}
	if      (*(strA - shiftA - counter) == '\0'
		  && *(strB - shiftB - counter) == '\0')
		return  0;
	else if (*(strA - shiftA - counter) == '\0')
		return -1;
	else if (*(strB - shiftB - counter) == '\0')
		return  1;
	else
		return COMPAR_ERROR;
}

int isLetter(char X) {
	return ((X >= ru_A && X <= ru_ya) || (X == ru_YO) || (X == ru_yo) );
}

int my_compare(int symbA, int symbB) {

	double myCodeA = 0.0;
	double myCodeB = 0.0;

	if 		(symbA == ru_YO) 
		myCodeA = ((double)(ru_YE + ru_ZHE)) / 2;
	else if (symbA == ru_yo)
		myCodeA = ((double)(ru_ye + ru_zhe)) / 2;
	else
		myCodeA = (double) symbA;

 	if 		(symbB == ru_YO) 
		myCodeA = ((double)(ru_YE + ru_ZHE)) / 2;
	else if (symbB == ru_yo)
		myCodeB = ((double)(ru_ye + ru_zhe)) / 2;
	else    
		myCodeB = (double) symbB;
	
	if 		((myCodeA - myCodeB) > tolerance)
		return 1;
	else if ((myCodeB - myCodeA) > tolerance)
		return 2;
	else
		return 0;
}
