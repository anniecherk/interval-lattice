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
    // ^ idk where the c++ 11 libraries are...

    bool overflow = false; // this is hacky, but I think it makes the cases below easier to read
    bool underflow = false;

    int smallest_overflow = 47; // 32+15 should loop around once, this is worse 'smallest' overflow
    int largest_underflow = -48; // -32-16

    for ( int i = 0 ; i < 4 ; ++i ) {
        if ((bounds[i] > 15) && (bounds[i] < smallest_overflow)){
            smallest_overflow = bounds[i];
            overflow = true;
        }
        if ((bounds[i] < -16) && (bounds[i] > largest_underflow)){
            largest_underflow = bounds[i];
            underflow = true;
        }
    }


    if( (overflow == false) && (underflow == false) )
        return interval(bounds[0], bounds[1]);
    else if( (overflow == true) && (underflow == false) ) //high is 15, low is wrapped around (lowest overflow)
        return interval(smallest_overflow-32, 15);
    else if( (overflow == false) && (underflow == true) ) //low is -16, high is wrapped around (highest underflow)
        return interval(-16, largest_underflow+32);
    else //both underflow & overflow -> should grab both end points
        return interval::top();
}



interval interval::bitwise_and(interval left, interval right){
    interval top = interval::top();
    if((left == top) && (right == top)){
        return top; //if any
    }
    return interval(left.getLow(), left.getHigh());
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//     tests!
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void normal_tests()
{
    // (3,4) - (-5, -4)
    interval result = interval::subtract(interval(3,4), interval(-5, -4));
    bool cond = ((result.getLow() == 7) && (result.getHigh() == 9));
    if(! cond){
        std::cout << "Failed test: <(3, 4) - (-5, -4)>.\nShould be (7, 9), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }

    // (2, 3) - (0, 0)
    result = interval::subtract(interval(2,3), interval(0, 0));
    cond = ((result.getLow() == 2) && (result.getHigh() == 3));
    if(! cond){
        std::cout << "Failed test: <(2, 3) - (0, 0)>.\nShould be (2, 3), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }

    // (2, 5) - (0, 2)
    result = interval::subtract(interval(2, 5), interval(0, 2));
    cond = ((result.getLow() == 0) && (result.getHigh() == 5));
    if(! cond){
        std::cout << "Failed test: <(2, 5) - (0, 2)>.\nShould be (0, 5), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }

}

void overflow_tests(){
    // (0, 1) - (-16, -15)
    interval result = interval::subtract(interval(0,1), interval(-16, -15));
    bool cond = ((result.getLow() == -16) && (result.getHigh() == 15));
    if(! cond){
        std::cout << "Failed test: <(0, 1) - (-16, -15)>.\nShould be (-15, 16), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }
}

void underflow_tests(){
    // (-16,-15) - (0, 1)
    interval result = interval::subtract(interval(-16,-15), interval(0, 1));
    bool cond = ((result.getLow() == -16) && (result.getHigh() == 15));
    if(! cond){
        std::cout << "Failed test: <(-16, -15) - (0, 1)>.\nShould be (-16, -15), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }
}

void under_overflow_tests(){
    // (3,4) - (-5, -4)
    interval result = interval::subtract(interval(3,4), interval(-5, -4));
    bool cond = ((result.getLow() == 7) && (result.getHigh() == 9));
    if(! cond){
        std::cout << "Failed test: <(3, 4) - (-5, -4)>.\nShould be (7, 9), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
main()
{   normal_tests();
    overflow_tests();
    underflow_tests();
    under_overflow_tests();

    std::cout << "All tests passed! \n";

    return 0;
}
