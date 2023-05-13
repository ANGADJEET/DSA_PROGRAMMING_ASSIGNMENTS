#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/common.h"
#include "pa1.h"

// don't remove these globals
static struct record *record_arr = NULL;
static size_t max_capacity = 0;
static size_t num_records = 0;



// don't modify this function
struct record *get_record_arr()
{
  return record_arr;
}

// don't modify this function
// compare two uids
// return -1 if uid1 < uid2
// return 0 if uid1 == uid2
// return 1 if uid1 > uid2
static int cmp_uid(char *uid1, char *uid2)
{
  int i;

  for (i = 0; i < MAX_LEN; i++) {
    if (uid1[i] > uid2[i]) {
      return 1;
    }
    else if (uid1[i] < uid2[i]) {
      return -1;
    }
  }
  return 0;
}

// don't modify this function
// compare two records when uid is key
// return -1 if record1 < record2
// return 0 if record1 == record2
// return 1 if record1 > record2
static int cmp_record(struct record *r1, struct record *r2)
{
  return cmp_uid(r1->uid, r2->uid);
}

// don't modify this function
static void *allocate_memory(size_t size)
{
  return __mymalloc(size);
}

// don't modify this function
static void free_memory(void *ptr)
{
  __myfree(ptr);
}

// ---------------- Add your code below this line ----------------- //

#define NOT_IMPLEMENTED { printf("%s still needs to be implemented!\n", __func__); abort(); }

// return the total number of records in the
// record_arr
size_t get_num_records()
{
return num_records;
}

// insert record r at the end of the record array
// if the record array is full, allocate
// a new buffer of double size, copy data
// from old buffer to the new buffer
// delete the old buffer
// make record_arr point to the new buffer
void insert_record(struct record r)
{
if(max_capacity == 0)
{
record_arr= (struct record  *)allocate_memory(sizeof(struct record));
//record_arr[0]=r;
//num_records++;
num_records =0;
max_capacity++;
}
  if(num_records == max_capacity)//TO CHECK IF THE ARRAY IS ALREADY FULL OR NOT.
  {
  max_capacity = 2*max_capacity;
    struct record *temp; //New buffer.
    int size = num_records;
    temp = (struct record*) allocate_memory(sizeof(struct record)*max_capacity);

    for(int i = 0; i<num_records; i++)
    {
      temp[i] = record_arr[i];
    }
    temp[num_records] = r;
    free_memory(record_arr);
    record_arr = temp;
    num_records++;
  }
  else
  {
    record_arr[num_records] = r;
    num_records++;
  }
}

// return the record corresponding to the uid
// use the linear search algorithm
// you can also use cmp_uid routine defined earlier
// if none of the records contain the input uid
// return a dummy record with status == -1
struct record search_record_linear(char uid[MAX_LEN])
{
  for(int i = 0; i<num_records; i++)
  {
    if(cmp_uid(record_arr[i].uid,uid) == 0)
    {
      return record_arr[i];
    }
  }

  struct record dummy_record;
  dummy_record.status = -1; // Return an empty record with status field -1 if no match is found
  return dummy_record;

}


// return the record corresponding to the uid
// use the binary search algorithm
// you can assume that the record_arr is alaredy sorted
// if none of the records contain the input uid
// return a dummy record with status == -1
struct record search_record_binary(char uid[MAX_LEN])
{
int high = num_records - 1;
int low = 0;
while(low<=high)
{
  int mid = low + (high - low)/2;

  if(cmp_uid(record_arr[mid].uid,uid) == 0)
  {
    return record_arr[mid];
  }

  else
  {
  if(cmp_uid(record_arr[mid].uid,uid) == -1)
    {low = mid + 1;}
  

  else
 {
   high = mid - 1;
 }
}
}
  struct record dummy_record;
  dummy_record.status = -1;
  return dummy_record;
}



// delete record correspondig to the uid
// if the corresponding record doesn't exist
// return a dummy record with status == -1
// Otherwise, return a copy of the record to the caller
// resize the record array in the following manner
// let's say, x is the size of the array
// and after deletion only x/4
// space in the array is occupied, in this
// case, allocate an array of size x/2,
// copy records from old array to the new
// array, and delete the old array,
// make record_arr point to the new array
struct record delete_record(char uid[MAX_LEN])
{
int x = num_records;
struct record temp;


for(int i = 0; i<num_records; i++)
{
if(cmp_uid(uid,record_arr[i].uid)==0)
{
temp = record_arr[i];
record_arr[i]=record_arr[num_records-1];
num_records--;
}
}
if (x ==num_records)
{
temp.status = -1;
return temp
;
}

if(num_records<=max_capacity/4)
{
max_capacity = max_capacity/2;

struct record *temp2 = (struct record*) allocate_memory(max_capacity*sizeof(struct record));

for(int j = 0; j<num_records; j++)
{
temp2[j] = record_arr[j];
}
free_memory(record_arr);
record_arr = temp2;
}
return temp;

}



// delete all records in the record_arr
// free record_arr


void delete_all_records()
{
  free_memory(record_arr);
  num_records= 0;
  max_capacity = 0;
}

int partition(int s, int e) {
  struct record pivot = record_arr[s];
  int cnt = 0;
  for (int i = s+1; i <= e; i++) {
    if (cmp_record(&record_arr[i], &pivot) <= 0) {
      cnt++;
    }
  }
  int pivotIndex = s + cnt;
  struct record temp = record_arr[pivotIndex];
  record_arr[pivotIndex] = record_arr[s];
  record_arr[s] = temp;
  int i = s, j = e;
  while (i < pivotIndex && j > pivotIndex) {
    while (cmp_record(&record_arr[i], &pivot) <= 0 && i < pivotIndex) {
      i++;
    }
    while (cmp_record(&record_arr[j], &pivot) > 0 && j > pivotIndex) {
      j--;
    }
    if (i < pivotIndex && j > pivotIndex) {
      struct record temp = record_arr[i];
      record_arr[i] = record_arr[j];
      record_arr[j] = temp;
      i++;
      j--;
    }
  }
  return pivotIndex;
}

void quickSort(int s, int e)
{
  if(s >= e)
  {
    return;
  }

  int p = partition(s,e);
  quickSort(s,p-1);
  quickSort(p+1,e);
}
// sort the record array using quick sort
// use cmp_record implementaion to compare two records
void sort_records_quick()
{
  quickSort(0,num_records-1);
}

void Merge(int l, int mid, int h) {
    int i = l, j = mid + 1, k = 0;
    struct record *B = (struct record*) allocate_memory((h-l+1) * sizeof(struct record));

    while (i <= mid && j <= h) {
        if (cmp_record(&record_arr[i], &record_arr[j]) == -1)
            B[k++] = record_arr[i++];
        else
            B[k++] = record_arr[j++];
    }
    for (; i <= mid; i++)
        B[k++] = record_arr[i];
    for (; j <= h; j++)
        B[k++] = record_arr[j];
    for (i = l, k = 0; i <= h; i++, k++)
        record_arr[i] = B[k];

    free_memory(B);
}

void MergeSort(int l, int h) {
    if (l < h) {
        int mid = (l + h) / 2;
        MergeSort(l, mid);
        MergeSort(mid + 1, h);
        Merge(l, mid, h);
    }
}

// sort the record_arr using merge sort
// use cmp_record implementaion to compare two records
void sort_records_merge()
{
  MergeSort(0,num_records-1);
}

// sort the record_arr using selection sort
// use cmp_record implementaion to compare two records
void sort_records_selection()
{
  int i, j, min_idx;

  for (i = 0; i < num_records - 1; i++)
  {
    min_idx = i;
    for (j = i + 1; j < num_records; j++)
    {
      if (cmp_record(&record_arr[j], &record_arr[min_idx]) == -1)
      {
        min_idx = j;
      }
    }
    if (min_idx != i)




    
    {
      struct record temp = record_arr[min_idx];
      record_arr[min_idx] = record_arr[i];
      record_arr[i] = temp;
    }
  }
}


// return the number of records corresponding to the input name
// use linear search to obtain the results
size_t get_num_records_with_name_linear(char name[MAX_LEN])
{
size_t count = 0;

for(int i = 0; i<num_records; i++)
{
if(strcmp(name,record_arr[i].name)==0)
{
count++;
}
}
return count;
}



int Partition(int s, int e) {
  struct record pivot = record_arr[s];
  int cnt = 0;
  for (int i = s+1; i <= e; i++) {
    if (strcmp(record_arr[i].name, pivot.name) <= 0) {
      cnt++;
    }
  }
  int pivotIndex = s + cnt;
  struct record temp = record_arr[pivotIndex];
  record_arr[pivotIndex] = record_arr[s];
  record_arr[s] = temp;
  int i = s, j = e;
  while (i < pivotIndex && j > pivotIndex) {
    while (strcmp(record_arr[i].name, pivot.name) <= 0 && i < pivotIndex) {
      i++;
    }
    while (strcmp(record_arr[j].name, pivot.name) > 0 && j > pivotIndex) {
      j--;
    }
    if (i < pivotIndex && j > pivotIndex) {
      struct record temp = record_arr[i];
      record_arr[i] = record_arr[j];
      record_arr[j] = temp;
      i++;
      j--;
    }
  }
  return pivotIndex;
}

void QuickSort(int s, int e)
{
  if(s >= e)
  {
    return;
  }

  int p = Partition(s,e);
  QuickSort(s,p-1);
  QuickSort(p+1,e);
}

void rearrange_data()
{
QuickSort(0,num_records-1);
}

// return the number of records corresponding to the input name
// use binary search to obtain the results
// you can assume that the caller has sorted
// data using name as key before making these queries
size_t get_num_records_with_name_binary(char name[MAX_LEN])
{
    int high = num_records - 1;
    int low = 0;
    size_t count = 0;
    
    while(low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(name, record_arr[mid].name);
        
        if(cmp == 0)
        {
            count++;
            // Check for more records with the same name to the left and right of mid
            for(int i = mid + 1; i < num_records && strcmp(name, record_arr[i].name) == 0; i++)
            {
                count++;
            }
            for(int i = mid - 1; i >= 0 && strcmp(name, record_arr[i].name) == 0; i--)
            {
                count++;
            }
            return count;
        }
        else if(cmp < 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    
    return count;
}
