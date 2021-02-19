#include "gtest/gtest.h"
#include "fifo.h"
#include <cstdio>
#include <iostream>
using namespace std;

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TFifo_Test: public ::testing::Test {

protected:
};


TEST_F(TFifo_Test, UINT8_T) {
	const size_t len = 4;
	uint8_t var = 0;
	TFifo<len, uint8_t> fifo;

	// Проверка начального состояния очереди
	ASSERT_FALSE(fifo.pop(var));

	// Проверка заполнения очереди
	for(size_t i = 0; i < len; i++) {
		ASSERT_TRUE(fifo.push((uint8_t) (i + 1))) << " i = " << i;
	}
	ASSERT_FALSE(fifo.push(var));

	// Проверка считывания очереди
	for(size_t i = 0; i < len; i++) {
		ASSERT_TRUE(fifo.pop(var));
		ASSERT_EQ((uint8_t) (i + 1), var);
	}
	ASSERT_FALSE(fifo.pop(var));

	// Проверка очистки очереди
	for(size_t i = 0; i < len; i++) {
		ASSERT_TRUE(fifo.push((uint8_t) (i + 1))) << " i = " << i;
	}
	fifo.flush();
	ASSERT_FALSE(fifo.pop(var));
}

TEST_F(TFifo_Test, FLOAT) {
	const size_t len = 4;
	float var = 0;
	TFifo<len, float> fifo;

	// Проверка начального состояния очереди
	ASSERT_FALSE(fifo.pop(var));

	// Проверка заполнения очереди
	for(size_t i = 0; i < len; i++) {
		ASSERT_TRUE(fifo.push((float) (i + 1))) << " i = " << i;
	}
	ASSERT_FALSE(fifo.push(var));

	// Проверка считывания очереди
	for(size_t i = 0; i < len; i++) {
		ASSERT_TRUE(fifo.pop(var));
		ASSERT_EQ((float) (i + 1), var);
	}
	ASSERT_FALSE(fifo.pop(var));

	// Проверка очистки очереди
	for(size_t i = 0; i < len; i++) {
		ASSERT_TRUE(fifo.push((float) (i + 1))) << " i = " << i;
	}
	fifo.flush();
	ASSERT_FALSE(fifo.pop(var));
}
