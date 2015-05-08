//==================================================================================================================|
// Created 2015.03.25 by Daniel L. Watkins
//
// Copyright (C) 2014-2015 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef _ValpineBase_Loadable_H
#define _ValpineBase_Loadable_H

#include "Property.h"

namespace vbase
{
    /**
     * @brief Manages the conceptual state of loading for a particular system.
     *
     * Users should publicly subclass Loadable.
     */
	class Loadable
	{
	public:
		virtual ~Loadable() {}

        /**
         * @brief pIsLoading True if the client is currently using a begin lock or if the client explicitly set to true.
         */
		//Property<bool> pIsLoading = false;
		Property_Set(bool, pIsLoading, false,
		{
			if (pIsLoading.raw() && !_newValue)
				pHasLoaded.raw() = true;

			pIsLoading.raw() = _newValue;
		})

                                            //TODO make this a ReadProperty since users of clients shouldn't be able
                                            //to modify this. Same wtih pHasLoaded.

        //TODO one issue with this Property<T> class is both read and write access must be on the same level.
        //Once I implement the various ReadProperty<T> and WriteProperty<T> things, those can be set to reference
        //a real property which can be declared on a more restricted level. For example:
        //         private: Property<float> _pAmount = 42.0f;
        //         public: ReadProperty<float> pAmount = &_pAmount;
        //I"m thinking WriteProperty<T> will be pointless though.

        /**
         * @brief pHasLoaded True if pIsLoading has gone from true to false. This does not necessarily mean whatever
         * was loaded is in a valid state. TODO add an error type so users can check for error that might have occured
         * while loading.
         */
        Property<bool> pHasLoaded = false;

		/**
		 * \brief RAII type for properly setting the pIsLoading property.
		 * Instantiate this with a pointer to the Loadable at the start of any root-level "loading" method. The
		 * pIsLoading property will be set to true upon construction, and fale upon destruction.
		 */
		class Begin
		{
		public:
			Begin() = delete;
			Begin(Loadable *parent) : mParent(parent) { mParent->pIsLoading = true; }
			~Begin() { mParent->pIsLoading = false; }

		private:
			Loadable *mParent = nullptr;
		};
	};
}

#endif
