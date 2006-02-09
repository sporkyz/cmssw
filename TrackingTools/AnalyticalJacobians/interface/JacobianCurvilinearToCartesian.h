#ifndef JacobianCurvilinearToCartesian_H
#define JacobianCurvilinearToCartesian_H

#include "Geometry/CommonDetAlgo/interface/AlgebraicObjects.h"

class GlobalTrajectoryParameters;

/** Class which calculates the Jacobian matrix of the transformation
 *  from the curvilinear to the Cartesian frame. The Jacobian is calculated
 *  during construction and thereafter cached, enabling reuse of the same
 *  Jacobian without calculating it again.
 */

class JacobianCurvilinearToCartesian {

 public:
  
  /** Constructor from global trajectory parameters. NB!! No default constructor exists!
   */
  
  JacobianCurvilinearToCartesian(const GlobalTrajectoryParameters& globalParameters);
  
  /** Access to Jacobian.
   */
  
  const AlgebraicMatrix& jacobian() const;


 private:
  
  AlgebraicMatrix theJacobian;

};  

#endif //JacobianCurvilinearToCartesian_H
