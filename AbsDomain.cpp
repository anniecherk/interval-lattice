#include <iostream>
#include <assert.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class interval {
  // 5-bit signed intervals: valid values for lo and hi are -16..15 and it is
  // required that hi >= lo. the bounds are inclusive.
    int low, high;

public:
  // these functions must trigger an assertion violation when presented with any
  // invalid interval, and must never return an invalid interval

  // requirement: be sound and fully precise
  // the second argument is to be subtracted from the first
  static interval subtract(interval, interval);  // static means there is no "this"
  static interval top() { return interval(-16, 15); }

  // requirement: be sound and don't always return top
  static interval bitwise_and(interval, interval);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // helpers
  interval(int x, int y);
  void is_valid_huh();    //validates the interval

  bool operator == (interval &other) {return((this->low == other.getLow()) && this->high == other.getHigh());}

  int getLow(){  return low; }
  int getHigh(){ return high; }
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

interval::interval(int x, int y){
    low = x;
    high = y;
    is_valid_huh();
}

void interval::is_valid_huh(){
    assert(low  <= high);
    assert(low  >= -16);
    assert(high <=  15);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

interval interval::subtract(interval left, interval right){

    int bounds[4] = {left.getLow()  - right.getHigh(),
                    left.getHigh() - right.getLow(),
                    left.getLow()  - right.getLow(),
                    left.getHigh() - right.getHigh()};

    // std::all_of(std::begin(bounds), std::end(bounds), [](int a){return ((a>=-16) && (a<15));}



    // for ( n=0 ; n<5 ; ++n )
    //   {
    //     result += foo[n];
    //   }

//    if()



    // if()             //no overflow
    //     return interval(low, high);
    //
    // // else if (low < -16) //underflow
    // //     low = 15 - (low + 16);
    // // else if (high > 15)
    // //     low = 2;
    // else
    return interval(left.getLow(), left.getHigh());
}

interval interval::bitwise_and(interval left, interval right){
    interval top = interval::top();
    if((left == top) && (right == top)){
        return top; //if any
    }
    return interval(left.getLow(), left.getHigh());
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void no_overflow_tests()
{
    interval three_four = interval(3,4);
    interval neg_five_neg_four = interval(-5, -4);

    interval result = interval::subtract(three_four, neg_five_neg_four);

    assert((result.getLow() == 0) && (result.getHigh() == 7));

}

void overflow_tests(){
    interval zero_one = interval(0,1);
    interval neg_sixteen_fifteen = interval(-16, 15);

    interval result = interval::subtract(zero_one, neg_sixteen_fifteen);

    assert((result.getLow() == -16) && (result.getHigh() == 15));
}


main()
{
    no_overflow_tests();
    // overflow_tests();

    std::cout << "done! \n";

    return 0;
}
