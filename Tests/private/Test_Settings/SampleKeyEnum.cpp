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

	using KUT = vbase::SettingsBase::KeyUpdateType;

	dvh[GraphicsWindowWidth] = { QVariant(1600), KUT::Pending };
	dvh[GraphicsWindowHeight] = { QVariant(900), KUT::Pending };
	dvh[GraphicsWindowIsFullscreen] = { QVariant(false), KUT::Pending };
	dvh[GraphicsWindowBackgroundColor] = { QVariant("0x000000"), KUT::Instant };

	return dvh;
}


SampleKeyClass::DefaultValuesHash SampleKeyClass::mDefaultValuesHash =
		SampleKeyClass::buildDefaultValuesHash();
