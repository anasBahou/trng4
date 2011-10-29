// Copyright (C) 2006 Heiko Bauke <heiko.bauke@physik.uni-magdeburg.de>
//  
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License in
// version 2 as published by the Free Software Foundation.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//  
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.
//  

#if !(defined TRNG_YARN3S_HPP)

#define TRNG_YARN3S_HPP

#include <ostream>
#include <istream>
#include <stdexcept>
#include <trng/utility.hpp>

namespace trng {
  
  class yarn3s;
  
  class yarn3s {
  public:
  
    // Uniform random number generator concept
    typedef long result_type;
    result_type operator()() const;
  private:
    static const result_type modulus;
  public:
    static const result_type min;
    static const result_type max;

    // Parameter and status classes
    class parameter_type;
    class status_type;

    class parameter_type {
      result_type a1, a2, a3;
      utility::power<2147462579l> g;
    public:
      parameter_type() :
        a1(0), a2(0), a3(0), g() { };
      parameter_type(result_type a1, result_type a2,
                     result_type a3,
		     result_type g) :
	a1(a1), a2(a2), a3(a3), g(g) { };

      friend class yarn3s;

      // Equality comparable concept
      friend bool operator==(const parameter_type &, const parameter_type &);
      friend bool operator!=(const parameter_type &, const parameter_type &);

      // Streamable concept
      template<typename char_t, typename traits_t>
      friend std::basic_ostream<char_t, traits_t> &
      operator<<(std::basic_ostream<char_t, traits_t> &out,
                 const parameter_type &P) {
        std::ios_base::fmtflags flags(out.flags());
        out.flags(std::ios_base::dec | std::ios_base::fixed |
                  std::ios_base::left);
        out << '('
            << P.a1 << ' ' << P.a2 << ' ' << P.a3 << ' ' << P.g
            << ')';
        out.flags(flags);
        return out;
      }

      template<typename char_t, typename traits_t>
      friend std::basic_istream<char_t, traits_t> &
      operator>>(std::basic_istream<char_t, traits_t> &in,
                 parameter_type &P) {
        parameter_type P_new;
        std::ios_base::fmtflags flags(in.flags());
        in.flags(std::ios_base::dec | std::ios_base::fixed |
                 std::ios_base::left);
        in >> utility::delim('(')
           >> P_new.a1 >> utility::delim(' ')
           >> P_new.a2 >> utility::delim(' ')
           >> P_new.a3 >> utility::delim(' ')
           >> P_new.g  >> utility::delim(')');
        if (in)
          P=P_new;
        in.flags(flags);
        return in;
      }

    };

    class status_type {
      result_type r1, r2, r3;
    public:
      status_type() : r1(0), r2(1), r3(1) { };
      status_type(result_type r1, result_type r2, 
		  result_type r3) :
        r1(r1), r2(r2), r3(r3) { };

      friend class yarn3s;

      // Equality comparable concept
      friend bool operator==(const status_type &, const status_type &);
      friend bool operator!=(const status_type &, const status_type &);

      // Streamable concept
      template<typename char_t, typename traits_t>
      friend std::basic_ostream<char_t, traits_t> &
      operator<<(std::basic_ostream<char_t, traits_t> &out,
                 const status_type &S) {
        std::ios_base::fmtflags flags(out.flags());
        out.flags(std::ios_base::dec | std::ios_base::fixed |
                  std::ios_base::left);
        out << '('
            << S.r1 << ' ' << S.r2 << ' ' << S.r3
            << ')';
        out.flags(flags);
        return out;
      }

      template<typename char_t, typename traits_t>
      friend std::basic_istream<char_t, traits_t> &
      operator>>(std::basic_istream<char_t, traits_t> &in,
                 status_type &S) {
        status_type S_new;
        std::ios_base::fmtflags flags(in.flags());
        in.flags(std::ios_base::dec | std::ios_base::fixed |
                 std::ios_base::left);
        in >> utility::delim('(')
           >> S_new.r1 >> utility::delim(' ')
           >> S_new.r2 >> utility::delim(' ')
           >> S_new.r3 >> utility::delim(')');
        if (in)
          S=S_new;
        in.flags(flags);
        return in;
      }

    };
      
    static const parameter_type trng1;
    
    // Random number engine concept
    explicit yarn3s(parameter_type=trng1);
    explicit yarn3s(unsigned long, parameter_type=trng1);
    template<typename gen>
    explicit yarn3s(gen &g, parameter_type P=trng1) : P(P), S() {
      seed(g);
    }

    void seed();
    void seed(unsigned long);
    template<typename gen>
    void seed(gen &g) {
      result_type r1=static_cast<unsigned long>(g())%
        static_cast<unsigned long>(modulus);
      result_type r2=static_cast<unsigned long>(g())%
        static_cast<unsigned long>(modulus);
      result_type r3=static_cast<unsigned long>(g())%
        static_cast<unsigned long>(modulus);
      S.r1=r1;
      S.r2=r2;
      S.r3=r3;
    }
    void seed(result_type, result_type, result_type);

    // Equality comparable concept
    friend bool operator==(const yarn3s &, const yarn3s &);
    friend bool operator!=(const yarn3s &, const yarn3s &);

    // Streamable concept
    template<typename char_t, typename traits_t>
    friend std::basic_ostream<char_t, traits_t> &
    operator<<(std::basic_ostream<char_t, traits_t> &out, const yarn3s &R) {
      std::ios_base::fmtflags flags(out.flags());
      out.flags(std::ios_base::dec | std::ios_base::fixed |
                std::ios_base::left);
      out << '[' << yarn3s::name() << ' ' << R.P << ' ' << R.S << ']';
      out.flags(flags);
      return out;
    }

   template<typename char_t, typename traits_t>
    friend std::basic_istream<char_t, traits_t> &
    operator>>(std::basic_istream<char_t, traits_t> &in, yarn3s &R) {
      yarn3s::parameter_type P_new;
      yarn3s::status_type S_new;
      std::ios_base::fmtflags flags(in.flags());
      in.flags(std::ios_base::dec | std::ios_base::fixed |
               std::ios_base::left);
      in >> utility::ignore_spaces();
      in >> utility::delim('[')
         >> utility::delim(yarn3s::name()) >> utility::delim(' ')
         >> P_new >> utility::delim(' ')
         >> S_new >> utility::delim(']');
      if (in) {
        R.P=P_new;
        R.S=S_new;
      }
      in.flags(flags);
      return in;
    }

    // Parallel random number generator concept
    void split(unsigned int, unsigned int);
    void jump2(unsigned int);
    void jump(unsigned long long);

    // Other useful methods
    static const char * name();
    long operator()(long) const;
//     bool boolean() const;
//     bool boolean(double) const;
//     double uniformco() const;
//     double uniformco(double, double) const;
//     double uniformoc() const;
//     double uniformoc(double, double) const;
//     double uniformoo() const;
//     double uniformoo(double, double) const;
//     double uniformcc() const;
//     double uniformcc(double, double) const;

  private:
    parameter_type P;
    mutable status_type S;
    static const char * const name_str;
    
    void backward();
    void step() const;
  };
    
  // Inline and template methods

  inline void yarn3s::step() const {
    unsigned long long t(static_cast<unsigned long long>(P.a1)*
			 static_cast<unsigned long long>(S.r1)+
			 static_cast<unsigned long long>(P.a2)*
			 static_cast<unsigned long long>(S.r2)+
			 static_cast<unsigned long long>(P.a3)*
			 static_cast<unsigned long long>(S.r3));
    t=(t&0x7fffffffull)+(t>>31)*21069;
    t=(t&0x7fffffffull)+(t>>31)*21069;
    if (t>=2147462579ull)
      t-=2147462579ull;
    S.r3=S.r2;  S.r2=S.r1;  S.r1=t;
  }
  
  inline yarn3s::result_type yarn3s::operator()() const {
    step();
    return (S.r1==0) ? 0 : P.g(S.r1);
  }
  
  inline long yarn3s::operator()(long x) const {
    return static_cast<long>(utility::uniformco(*this)*x);
  }

//   inline bool yarn3s::boolean() const {
//     return operator()()<=modulus/2;
//   }

//   inline bool yarn3s::boolean(double p) const {
//     return operator()()<modulus*p;
//   }
  
//   inline double yarn3s::uniformco() const {
//     return static_cast<double>(operator()())/static_cast<double>(modulus);
//   }

//   inline double yarn3s::uniformco(double a, double b) const {
//     return uniformco()*(b-a)+a;
//   }
  
//   inline double yarn3s::uniformoc() const {
//     return (static_cast<double>(operator()())+1.0)/static_cast<double>(modulus);
//   }

//   inline double yarn3s::uniformoc(double a, double b) const {
//     return uniformoc()*(b-a)+a;
//   }
  
//   inline double yarn3s::uniformoo() const {
//     return (static_cast<double>(operator()())+1.0)/(static_cast<double>(modulus)+1.0);
//   }

//   inline double yarn3s::uniformoo(double a, double b) const {
//     return uniformoo()*(b-a)+a;
//   }

//   inline double yarn3s::uniformcc() const {
//     return static_cast<double>(operator()())/(static_cast<double>(modulus)-1.0);
//   }

//   inline double yarn3s::uniformcc(double a, double b) const {
//     return uniformcc()*(b-a)+a;
//   }
    
}
  
#endif
