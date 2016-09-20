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
    bool cond = low  <= high;
    if(!cond){
        std::cout << "low <= high failed for low= " << low << ", high= " << high;
        assert(cond);
    }

    cond = low  >= -16;
    if(!cond){
        std::cout << "low >= 16 failed for " << low << ", high= " << high;
        assert(cond);
    }

    cond = high <=  15;
    if(!cond){
        std::cout << "high <= 15 failed for low= " << low << ", high= " << high;
        assert(cond);
    }

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

interval interval::subtract(interval left, interval right){
    int low = left.getLow()  - right.getHigh();
    int high = left.getHigh() - right.getLow();

    if ((low >= -16) && (high <= 15)) //all quite
        return interval(low, high);
    if ((low < -16) && (high < -16)) //totally below the lower bounds
        return interval(low+32, high+32); //shift into range
    if ((low > 15) && (high > 15)) //totally above the lower bounds
        return interval(low-32, high-32); //shift into range
    else
        return interval::top(); // otherwise we straddle a boundry, so it's just top.
}



interval interval::bitwise_and(interval left, interval right){
    if ((left.getLow() < 0) &&  (right.getLow() < 0)
    && (left.getHigh() < 0) && (right.getHigh() < 0))
        return interval(-16, -1); //if both intervals are totally negative, then the top bit will be and'ed to 1

    if ((left.getLow() >= 0) &&  (right.getLow() >= 0)
    && (left.getHigh() >= 0) && (right.getHigh() >= 0))
        return interval(0, 15); //if both intervals are totally non-negative, then the top bit will be and'ed to 0

    return interval::top(); //"to do"
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
    interval result = interval::subtract(interval(0,1), interval(-16, -15)); //(0--16 overflows)
    bool cond = ((result.getLow() == -16) && (result.getHigh() == 15));
    if(! cond){
        std::cout << "Failed test: <(0, 1) - (-16, -15)>.\nShould be (-15, 16), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }

    // tricky!!
    // (14,15) - (-3, -4)
    result = interval::subtract(interval(14,15), interval(-4, -3));
    cond = ((result.getLow() == -15) && (result.getHigh() == -13));
    if(! cond){
        std::cout << "Failed test: <(14, 15) - (-4, -3)>.\nShould be (-15, -13), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
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

    // tricky!!
    // (-16,-15) - (3, 4)
    result = interval::subtract(interval(-16,-15), interval(3, 4));
    cond = ((result.getLow() == 12) && (result.getHigh() == 14));
    if(! cond){
        std::cout << "Failed test: <(-16, -15) - (3, 4)>.\nShould be (12, 14), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }
}

void under_overflow_tests(){
    // (-16, 2) - (-15, 1)
    interval result = interval::subtract(interval(-16, 3), interval(-15, 3)); //(-16-3 underflows, 3--15 overflows)
    bool cond = ((result.getLow() == -16) && (result.getHigh() == 15));
    if(! cond){
        std::cout << "Failed test: <(-16, 3) - (-15, 3)>.\nShould be (-16, 15), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void bitwise_and_test_pos_neg(){
    // (-16,-14) & (-5, -4)
    interval result = interval::bitwise_and(interval(-16,-14), interval(-5, -4));
    bool cond = ((result.getLow() == -16) && (result.getHigh() == -1));
    if(! cond){
        std::cout << "Failed test: <(-16, -14) & (-5, -4)>.\nShould be (-16, -1), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }

    // (1, 5) & (3, 9)
    result = interval::bitwise_and(interval(1,5), interval(3, 9));
    cond = ((result.getLow() == 0) && (result.getHigh() == 15));
    if(! cond){
        std::cout << "Failed test: <(1, 5) & (3, 9)>.\nShould be (0, 15), actually was (" << result.getLow() << ", " << result.getHigh() << ")\n";
        assert(cond);
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
main()
{   normal_tests();  //subtraction
    overflow_tests();
    underflow_tests();
    under_overflow_tests();

    bitwise_and_test_pos_neg();

    std::cout << "All tests passed! \n";

    return 0;
}
