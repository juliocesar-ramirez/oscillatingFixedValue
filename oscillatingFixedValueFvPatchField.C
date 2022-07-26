/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2022 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "mathematicalConstants.H"
#include "oscillatingFixedValueFvPatchField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
Foam::scalar Foam::oscillatingFixedValueFvPatchField<Type>::t() const
{
    return this->db().time().timeOutputValue();
}

template<class Type>
Foam::scalar Foam::oscillatingFixedValueFvPatchField<Type>::oscillatingPart() const
{
    scalar frecuenciaAngular=(constant::mathematical::twoPi/periodo_);
    return amplitud_*sin(frecuenciaAngular*t());
}



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template <class Type>
Foam::oscillatingFixedValueFvPatchField<Type>::
    oscillatingFixedValueFvPatchField(const fvPatch &p,
                                      const DimensionedField<Type, volMesh> &iF)
    : fixedValueFvPatchField<Type>(p, iF), scalarData_(0.0), amplitud_(0.0),
      periodo_(0.0), data_(Zero), valorMedio_(Zero), fieldData_(p.size(), Zero) {}

template<class Type>
Foam::oscillatingFixedValueFvPatchField<Type>::
oscillatingFixedValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<Type>(p, iF),
    scalarData_(dict.lookup<scalar>("scalarData")),
    amplitud_(dict.lookup<scalar>("scalarData")),
    periodo_(dict.lookup<scalar>("scalarData")),
    data_(dict.lookup<Type>("data")),
    valorMedio_(dict.lookup<Type>("data")),
    fieldData_("fieldData", dict, p.size())
{


    fixedValueFvPatchField<Type>::evaluate();

    /*
    // Initialise with the value entry if evaluation is not possible
    fvPatchField<Type>::operator=
    (
        Field<Type>("value", dict, p.size())
    );
    */
}


template<class Type>
Foam::oscillatingFixedValueFvPatchField<Type>::
oscillatingFixedValueFvPatchField
(
    const oscillatingFixedValueFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
    scalarData_(ptf.scalarData_),
    amplitud_(ptf.scalarData_),
    periodo_(ptf.scalarData_),
    data_(ptf.data_),
    valorMedio_(ptf.data_),
    fieldData_(mapper(ptf.fieldData_))
{}

template <class Type>
Foam::oscillatingFixedValueFvPatchField<Type>::
    oscillatingFixedValueFvPatchField(
        const oscillatingFixedValueFvPatchField<Type> &ptf,
        const DimensionedField<Type, volMesh> &iF)
        : fixedValueFvPatchField<Type>(ptf, iF), scalarData_(ptf.scalarData_), amplitud_(ptf.amplitud_),periodo_(ptf.periodo_),
          data_(ptf.data_),valorMedio_(ptf.valorMedio_), fieldData_(ptf.fieldData_) {}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * *
// * //

template <class Type>
void Foam::oscillatingFixedValueFvPatchField<Type>::autoMap(
    const fvPatchFieldMapper &m) {
  fixedValueFvPatchField<Type>::autoMap(m);
  m(fieldData_, fieldData_);
}

template<class Type>
void Foam::oscillatingFixedValueFvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    fixedValueFvPatchField<Type>::rmap(ptf, addr);

    const oscillatingFixedValueFvPatchField<Type>& tiptf =
        refCast<const oscillatingFixedValueFvPatchField<Type>>(ptf);

    fieldData_.rmap(tiptf.fieldData_, addr);
}


template<class Type>
void Foam::oscillatingFixedValueFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    fixedValueFvPatchField<Type>::operator==
    (
        valorMedio_*(1+oscillatingPart())
    );


    fixedValueFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void Foam::oscillatingFixedValueFvPatchField<Type>::write
(
    Ostream& os
) const
{
    fvPatchField<Type>::write(os);
    writeEntry(os, "scalarData", scalarData_);
    writeEntry(os, "amplitud", amplitud_);
    writeEntry(os, "periodo", periodo_);
    writeEntry(os, "data", data_);
    writeEntry(os, "valorMedio", valorMedio_);
    writeEntry(os, "fieldData", fieldData_);
    writeEntry(os, "value", *this);
}



// ************************************************************************* //
