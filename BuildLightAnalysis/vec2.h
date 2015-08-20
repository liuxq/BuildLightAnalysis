//-----------------------------------------------------------------------------
// File : vec2f.hpp
//-----------------------------------------------------------------------------
// GLVU : Copyright 1997 - 2002 
//        The University of North Carolina at Chapel Hill
//-----------------------------------------------------------------------------
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that the above copyright notice appear in all copies
// and that both that copyright notice and this permission notice
// appear in supporting documentation.  Binaries may be compiled with
// this software without any royalties or restrictions.
//
// The University of North Carolina at Chapel Hill makes no representations 
// about the suitability of this software for any purpose. It is provided 
// "as is" without express or implied warranty.

//==========================================================================
// Vec2.hpp : 3d vector class template. Works for any integer or real type.
//==========================================================================

#pragma once

#include <stdio.h>
#include <math.h>

/**
 * @class Vec2
 * @brief A templated 2-vector class
 *
 * Everybody in graphics has to write their own basic 3-vector class.
 * And we're no exception.  This one uses templates, so that with one
 * definition of the class you can have a 3-vector of floats or doubles
 * depending on how precise you are, or you can make 3-vectors of ints
 * or unsigned char, handy for representing colors.
 *
 * A couple of typedefs for common instatiations are provided by
 * default: Vec2f, and Vec2d, the float and double versions,
 * respectively.
 *
 */

template<class Type>
class Vec2
{
  public:
    Type x, y;  ///< The storage for the three components of the vector

    /// Default constructor.
    /// Note: does \em not initialize x, y, and z!
    Vec2 (void) 
      {};
    /// Three component constructor
    Vec2 (const Type X, const Type Y) 
      { x=X; y=Y; };
    /// Copy constructor
    Vec2 (const Vec2& v) 
      { x=v.x; y=v.y; };
    /// Construct from array
    Vec2 (const Type v[2])
      { x=v[0]; y=v[1]; };
    /// Set from components
    void Set (const Type X, const Type Y)
      { x=X; y=Y; }
    /// Set from array
    void Set (const Type v[2])
      { x=v[0]; y=v[1]; };

    operator Type*()                            /// Type * CONVERSION
      { return (Type *)&x; }
    operator const Type*() const                /// CONST Type * CONVERSION
      { return &x; }

    bool operator == (const Vec2& A) const       /// COMPARISON (==)
      { return (x==A.x && y==A.y); }
    bool operator != (const Vec2& A) const       /// COMPARISON (!=)
      { return (x!=A.x || y!=A.y); }

    bool operator > (const Vec2& A) const        /// ALL greater than
      { return (x>A.x && y>A.y); }
    bool operator < (const Vec2& A) const        /// ALL less than
      { return ((x<A.x) && (y<A.y)); }

    Vec2& operator = (const Vec2& A)            /// ASSIGNMENT (=)
      { x=A.x; y=A.y;
        return(*this);  };
    Vec2 operator + (const Vec2& A) const       /// ADDITION (+)
      { Vec2 Sum(x+A.x, y+A.y); 
        return(Sum); };
    Vec2 operator - (const Vec2& A) const       /// SUBTRACTION (-)
      { Vec2 Diff(x-A.x, y-A.y);
        return(Diff); };
    Type operator * (const Vec2& A) const       /// DOT-PRODUCT (*)
      { Type DotProd = x*A.x+y*A.y; 
        return(DotProd); };
    Vec2 operator / (const Vec2& A) const       /// CROSS-PRODUCT (/)
      { Vec2 CrossProd(y*A.z-z*A.y, z*A.x-x*A.z);
        return(CrossProd); };
    Vec2 operator ^ (const Vec2& A) const       /// ALSO CROSS-PRODUCT (^)
      { Vec2 CrossProd(y*A.z-z*A.y, z*A.x-x*A.z);
        return(CrossProd); };
    Vec2 operator * (const Type s) const        /// MULTIPLY BY SCALAR V*s (*)
      { Vec2 Scaled(x*s, y*s); 
        return(Scaled); };
    Vec2 operator / (const Type s) const        /// DIVIDE BY SCALAR (/)
      { Vec2 Scaled(x/s, y/s);
        return(Scaled); };
    Vec2 operator & (const Vec2& A) const       /// COMPONENT MULTIPLY (&)
      { Vec2 CompMult(x*A.x, y*A.y);
        return(CompMult); }

    friend inline Vec2 operator *(Type s, const Vec2& v)  /// SCALAR MULT s*V
      { return Vec2(v.x*s, v.y*s); }

    Vec2& operator += (const Vec2& A)    /// ACCUMULATED VECTOR ADDITION (+=)
      { x+=A.x; y+=A.y;
        return *this;}
    Vec2& operator -= (const Vec2& A)    /// ACCUMULATED VECTOR SUBTRACTION (-=)
      { x-=A.x; y-=A.y;
        return *this; }
    Vec2& operator *= (const Type s)     /// ACCUMULATED SCALAR MULT (*=)
      { x*=s; y*=s;
        return *this; }
    Vec2& operator /= (const Type s)     /// ACCUMULATED SCALAR DIV (/=)
      { x/=s; y/=s;
        return *this; }
    Vec2& operator &= (const Vec2& A)  /// ACCUMULATED COMPONENT MULTIPLY (&=)
      { x*=A.x; y*=A.y; return *this; }
    Vec2 operator - (void) const        /// NEGATION (-)
      { Vec2 Negated(-x, -y);
        return(Negated); };

/*
    const Type& operator [] (const int i) const // ALLOWS VECTOR ACCESS AS AN ARRAY.
      { return( (i==0)?x:((i==1)?y:z) ); };
    Type & operator [] (const int i)
      { return( (i==0)?x:((i==1)?y:z) ); };
*/

    Type Length (void) const                     /// LENGTH OF VECTOR
      { return ((Type)sqrt(x*x+y*y)); };
    Type LengthSqr (void) const                  /// LENGTH OF VECTOR (SQUARED)
      { return (x*x+y*y); };
    Vec2& Normalize (void)                       /// NORMALIZE VECTOR
      { Type L = Length();                       //  CALCULATE LENGTH
        if (L>0) { x/=L; y/=L;}           //  DIV COMPONENTS BY LENGTH
        return *this;
      };                                         

    /// Returns the 'star' matrix for a vector
    /** This is the skew-symmetric matrix \b A such that 
     *  \b A \b v == \p this x \b v 
     * (the cross-product of \p this and \b v), for any vector \b v.
     *  The matrix looks like this given vector (x,y,z):
     *  @verbatim
            | 0  -z   y|
            | z   0  -x|
            |-y   x   0|
        @endverbatim
     * 
     * Return format is just an array in row-major (OpenGL/Fortran) order. 
     * That is [0, -z, y, z, 0, -x, -y, x, 0].
     */


    /// Update \p Min and \p Max to enclose \p this
    /** A very handy routine for working with min-max or axis aligned 
     *  bounding boxes.
     */
    void UpdateMinMax(Vec2 &Min, Vec2 &Max) const
    {
      if (x<Min.x) Min.x=x; if (x>Max.x) Max.x=x;
      if (y<Min.y) Min.y=y; if (y>Max.y) Max.y=y;
    }

    /// Construct an orthonormal basis from \p this
    /** Compute two unit vectors \p U and \p V that are orthogonal
     * to this vector and to each other.  Note that \p *this need
     * not be a unit vector.
     *
     * The algorithm works as follows:
     * Find smallest component of L (this), zero it, 
     * negate one of the other two and swap them.  Then normalize.
     * Ex. if x1 is the smallest, assign (x2,y2,z2):=(0,z1,-y1)
     * Clearly now v1 dot v2 = x1*0 + y1*z1 + z1*-y1 = 0;
     * Zeroing out the smallest means that the magnitude of 
     * the remaining vector will be as  big as possible so that 
     * when we normalize, we are safe from dividing by anything
     * close to zero (unless *this was near 0 magnitude to 
     * begin with, in which case lack of precision can't be avoided)
     */
    /// This is a handy way to get the zero vector just use Vec2<Type>::ZERO
    static Vec2 ZERO;
};

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<double> Vertex;
typedef Vec2<int> VertexPos;

template<class Type> Vec2<Type> Vec2<Type>::ZERO = Vec2<Type>(0,0);
