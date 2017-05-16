/*
  Copyright (C) 2014 - 2017 by the authors of the ASPECT code.

  This file is part of ASPECT.

  ASPECT is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  ASPECT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ASPECT; see the file doc/COPYING.  If not see
  <http://www.gnu.org/licenses/>.
*/


#ifndef _aspect_boundary_composition_function_h
#define _aspect_boundary_composition_function_h

#include <aspect/boundary_composition/interface.h>
#include <aspect/simulator_access.h>
#include <aspect/utilities.h>

#include <deal.II/base/parsed_function.h>

namespace aspect
{
  namespace BoundaryComposition
  {
    using namespace dealii;

    /**
     * A class that implements boundary composition based on a functional
     * description provided in the input file.
     *
     * @ingroup BoundaryCompositions
     */
    template <int dim>
    class Function : public Interface<dim>, public SimulatorAccess<dim>
    {
      public:
        /**
         * Return the boundary composition as a function of position and time.
         *
         * @copydoc aspect::BoundaryComposition::Interface::boundary_composition()
         */
        virtual
        double boundary_composition (const types::boundary_id boundary_indicator,
                                     const Point<dim> &position, const unsigned int n_comp) const;
        /**
         * A function that is called at the beginning of each time step to
         * indicate what the model time is for which the boundary values will
         * next be evaluated. For the current class, the function passes to
         * the parsed function what the current time is.
         */
        virtual
        void
        update ();

        /**
         * Return the minimal composition on that part of the boundary on
         * which Dirichlet conditions are posed. Note: This is left over from
         * temperature formulations, and may need to be removed later if its
         * efficacy is not proven.
         */
        virtual
        double minimal_composition (const std::set<types::boundary_id> &fixed_boundary_ids) const;

        /**
         * Return the maximal composition on that part of the boundary on
         * which Dirichlet conditions are posed. Note: This is left over from
         * temperature formulations, and may need to be removed later if its
         * efficacy is not proven.
         */
        virtual
        double maximal_composition (const std::set<types::boundary_id> &fixed_boundary_ids) const;

        /**
         * Declare the parameters this class takes through input files. The
         * default implementation of this function does not describe any
         * parameters. Consequently, derived classes do not have to overload
         * this function if they do not take any runtime parameters.
         */
        static
        void
        declare_parameters (ParameterHandler &prm);

        /**
         * Read the parameters this class declares from the parameter file.
         * The default implementation of this function does not read any
         * parameters. Consequently, derived classes do not have to overload
         * this function if they do not take any runtime parameters.
         */
        virtual
        void
        parse_parameters (ParameterHandler &prm);

      private:
        /**
         * A function object representing the compositional fields.
         */
        std_cxx11::unique_ptr<Functions::ParsedFunction<dim> > function;

        /**
         * Composition at the inner and outer boundaries.
         */
        double min_composition;
        double max_composition;

        /**
         * The coordinate representation to evaluate the function. Possible
         * choices are depth, cartesian and spherical.
         */
        Utilities::Coordinates::CoordinateSystem coordinate_system;
    };
  }
}


#endif
