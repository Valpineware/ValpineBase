#include "SampleKeyEnum.h"


vbase::SettingsBase::MetaKeyInfo SampleKeyClass::metaKeyInfoForKey(SampleKeyClass::KeyEnum key)
{
	auto valueIter = mDefaultValuesHash.find(key);

	if (valueIter != mDefaultValuesHash.end())
		return valueIter.value();

	return vbase::SettingsBase::MetaKeyInfo();
}


SampleKeyClass::DefaultValuesHash SampleKeyClass::buildDefaultValuesHash()
{
	DefaultValuesHash dvh;

	dvh[GraphicsWindowWidth] = { QVariant(1600) };
	dvh[GraphicsWindowHeight] = { QVariant(900) };
	dvh[GraphicsWindowIsFullscreen] = { QVariant(false) };
	dvh[GraphicsWindowBackgroundColor] = { QVariant("0x000000") };

	return dvh;
}


SampleKeyClass::DefaultValuesHash SampleKeyClass::mDefaultValuesHash =
		SampleKeyClass::buildDefaultValuesHash();
