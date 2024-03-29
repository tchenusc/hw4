#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // AVL Tree Tests
    AVLTree<int, std::string> d;
	std::pair<int, std::string> item5(5, "five");
	std::pair<int, std::string> item3(3, "three");
	std::pair<int, std::string> item7(7, "seven");
	std::pair<int, std::string> item4(4, "four");
	std::pair<int, std::string> item6(6, "six");
	std::pair<int, std::string> item9(9, "nine");
	std::pair<int, std::string> item10(10, "ten");
	std::pair<int, std::string> item2(2, "two");
	std::pair<int, std::string> item8(8, "eight");
	std::pair<int, std::string> item13(13, "thirteen");
	std::pair<int, std::string> item11(11, "eleven");
	std::pair<int, std::string> item12(12, "twelve");
	std::pair<int, std::string> item15(15, "fifteen");
	std::pair<int, std::string> item14(14, "fourteen");
	std::pair<int, std::string> item16(16, "sixteen");
	std::pair<int, std::string> item17(17, "seventeen");

	d.insert(item10);
	d.insert(item3);
	d.insert(item2);
	d.insert(item15);
	d.insert(item12);
	d.insert(item11);
	d.insert(item14);
	d.insert(item13);
	d.insert(item16);
	d.insert(item17);
    d.print();

    d.remove(5);

    d.print();
    return 0;
}
