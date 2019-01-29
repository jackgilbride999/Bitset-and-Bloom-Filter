#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "bitset.h"

// return the value of bit index.
int get_bit(unsigned num, int index){
	unsigned mask = (1<<(sizeof(unsigned)*8-1-index));
	unsigned value = num&mask;
	if(value==0){
		return 0;
	}
	else{
		return 1;
	}
}

// set bit index of input.
unsigned set_bit(unsigned input, int index){
	unsigned mask = (1<<(sizeof(unsigned)*8-1-index));
	unsigned new_word = input | mask;
	return new_word;
}

// clear bit index of input.
unsigned clear_bit(unsigned input, int index){
	unsigned mask = ~(1<<(sizeof(unsigned)*8-1-index));
	return input&mask;
}

// flip bit index of input
unsigned flip_bit(unsigned input, int index){
	unsigned mask = (1<<(sizeof(unsigned)*8-1-index));
	return input ^ mask;
}

// create a new, empty bit vector set with a universe of 'size' items.
struct bitset * bitset_new(int size){
	int i, bits_per_word, size_in_words;
	struct bitset * result;
	result = malloc(sizeof(struct bitset));
	bits_per_word = sizeof(unsigned)*8;
	size_in_words = size/bits_per_word;
	if(size%bits_per_word!=0){
		size_in_words++;
	}
	result->bits=malloc(sizeof(unsigned)*size_in_words);
	for(i=0; i<size_in_words; i++){
		result->bits[i]=0;
	}
	result->bits_per_word=bits_per_word;
	result->size_in_bits=size;
	result->size_in_words=size_in_words;
	return result;
}

// get the size of the universe of items that could be stored in the set. 
int bitset_size(struct bitset * this){
	return this->size_in_bits;
}

// get the number of items that are stored in the set
int bitset_cardinality(struct bitset * this){
	int count = 0;
	for(int word_index=0; word_index<this->size_in_words; word_index++){
		for(int bit_index=0; bit_index<this->bits_per_word; bit_index++){
			count+= get_bit(this->bits[word_index], bit_index);
		}
	}
	return count;
}

// check to see if an item is in the set. 
int bitset_lookup(struct bitset * this, int item){
	assert(item>=0 && item<this->size_in_bits);
	int word_index=item/this->bits_per_word;
	int bit_index=item%this->bits_per_word;
	return get_bit(this->bits[word_index], bit_index);
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set. 
void bitset_add(struct bitset * this, int item){ 
	assert(item>=0 && item<this->size_in_bits);
	int word_index=item/(this->bits_per_word);
	int bit_index=item%(this->bits_per_word);
	this->bits[word_index]=set_bit(this->bits[word_index], bit_index);
}

// remove an item with number 'item' from the set. 
void bitset_remove(struct bitset * this, int item){
	assert(item>=0 && item<this->size_in_bits);
	int word_index=item/this->bits_per_word;
	int bit_index=item%this->bits_per_word;
	this->bits[word_index] = clear_bit(this->bits[word_index], bit_index);
}

// place the union of src1 and src2 into dest. 
void bitset_union(struct bitset * dest, struct bitset * src1,
		  struct bitset * src2){
	for(int i=0; i<dest->size_in_words; i++){
		dest->bits[i]=0;									// empty dest
	}
	int i;
	assert(src1->size_in_bits==src2->size_in_bits);
	for(i=0; i<src1->size_in_words; i++){
		dest->bits[i]=src1->bits[i]|src2->bits[i];
	}
}

// place the intersection of src1 and src2 into dest. 
void bitset_intersect(struct bitset * dest, struct bitset * src1,
                  struct bitset * src2){
	for(int i=0; i<dest->size_in_words; i++){
		dest->bits[i]=0;									// empty dest
	}
	int i;
	assert(src1->size_in_bits==src2->size_in_bits);
	for(i=0; i<src1->size_in_words; i++){
		dest->bits[i]=src1->bits[i]&src2->bits[i];
	}
}

// print the contents of the bitset
void bitset_print(struct bitset * this)
{
  int i;
  int size = bitset_size(this);
  for ( i = 0; i < size; i++ ) {
    if ( bitset_lookup(this, i) == 1 ) {
      printf("%d ", i);
    }
  }
  printf("\n");
}
