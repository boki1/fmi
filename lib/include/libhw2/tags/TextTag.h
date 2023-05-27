#ifndef FMI_OOP_HW2_TEXTTAG_H
#define FMI_OOP_HW2_TEXTTAG_H

namespace libhw2 {

/// Base class for the "tag" hierarchy.
/// Each derived class specifies additional data other than the "regular" info.
class TextTag {

public:
	virtual ~TextTag() noexcept = default;
};

}

#endif // FMI_OOP_HW2_TEXTTAG_H
