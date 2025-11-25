#pragma once


#include "uint32_t.h"

#ifndef BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH
#	define BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH 32
#endif // BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH

#ifndef BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ
#	define BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ 20
#endif // BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ

#ifndef BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM
#	define BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM 9
#endif // BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM

// Cannot be configured via compiler options.
#define BGFX_CONFIG_MAX_PROGRAMS (1<<BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM)
//static_assert(bx::isPowerOf2(BGFX_CONFIG_MAX_PROGRAMS), "BGFX_CONFIG_MAX_PROGRAMS must be power of 2.");

#ifndef BGFX_CONFIG_MAX_VIEWS
#	define BGFX_CONFIG_MAX_VIEWS 256
#endif // BGFX_CONFIG_MAX_VIEWS
//static_assert(bx::isPowerOf2(BGFX_CONFIG_MAX_VIEWS), "BGFX_CONFIG_MAX_VIEWS must be power of 2.");

constexpr uint8_t  kSortKeyViewNumBits = uint8_t(31 - bx::uint32_cntlz(BGFX_CONFIG_MAX_VIEWS));
constexpr uint8_t  kSortKeyViewBitShift = 64 - kSortKeyViewNumBits;
constexpr uint64_t kSortKeyViewMask = uint64_t(BGFX_CONFIG_MAX_VIEWS - 1) << kSortKeyViewBitShift;

constexpr uint8_t  kSortKeyDrawBitShift = kSortKeyViewBitShift - 1;
constexpr uint64_t kSortKeyDrawBit = uint64_t(1) << kSortKeyDrawBitShift;

//
constexpr uint8_t  kSortKeyDrawTypeNumBits = 2;
constexpr uint8_t  kSortKeyDrawTypeBitShift = kSortKeyDrawBitShift - kSortKeyDrawTypeNumBits;
constexpr uint64_t kSortKeyDrawTypeMask = uint64_t(3) << kSortKeyDrawTypeBitShift;

constexpr uint64_t kSortKeyDrawTypeProgram = uint64_t(0) << kSortKeyDrawTypeBitShift;
constexpr uint64_t kSortKeyDrawTypeDepth = uint64_t(1) << kSortKeyDrawTypeBitShift;
constexpr uint64_t kSortKeyDrawTypeSequence = uint64_t(2) << kSortKeyDrawTypeBitShift;

//
constexpr uint8_t  kSortKeyTransNumBits = 2;

constexpr uint8_t  kSortKeyDraw0BlendShift = kSortKeyDrawTypeBitShift - kSortKeyTransNumBits;
constexpr uint64_t kSortKeyDraw0BlendMask = uint64_t(0x3) << kSortKeyDraw0BlendShift;

constexpr uint8_t  kSortKeyDraw0ProgramShift = kSortKeyDraw0BlendShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM;
constexpr uint64_t kSortKeyDraw0ProgramMask = uint64_t(BGFX_CONFIG_MAX_PROGRAMS - 1) << kSortKeyDraw0ProgramShift;

constexpr uint8_t  kSortKeyDraw0DepthShift = kSortKeyDraw0ProgramShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH;
constexpr uint64_t kSortKeyDraw0DepthMask = ((uint64_t(1) << BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH) - 1) << kSortKeyDraw0DepthShift;

//
constexpr uint8_t  kSortKeyDraw1DepthShift = kSortKeyDrawTypeBitShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH;
constexpr uint64_t kSortKeyDraw1DepthMask = ((uint64_t(1) << BGFX_CONFIG_SORT_KEY_NUM_BITS_DEPTH) - 1) << kSortKeyDraw1DepthShift;

constexpr uint8_t  kSortKeyDraw1BlendShift = kSortKeyDraw1DepthShift - kSortKeyTransNumBits;
constexpr uint64_t kSortKeyDraw1BlendMask = uint64_t(0x3) << kSortKeyDraw1BlendShift;

constexpr uint8_t  kSortKeyDraw1ProgramShift = kSortKeyDraw1BlendShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM;
constexpr uint64_t kSortKeyDraw1ProgramMask = uint64_t(BGFX_CONFIG_MAX_PROGRAMS - 1) << kSortKeyDraw1ProgramShift;

//
constexpr uint8_t  kSortKeyDraw2SeqShift = kSortKeyDrawTypeBitShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ;
constexpr uint64_t kSortKeyDraw2SeqMask = ((uint64_t(1) << BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ) - 1) << kSortKeyDraw2SeqShift;

constexpr uint8_t  kSortKeyDraw2BlendShift = kSortKeyDraw2SeqShift - kSortKeyTransNumBits;
constexpr uint64_t kSortKeyDraw2BlendMask = uint64_t(0x3) << kSortKeyDraw2BlendShift;

constexpr uint8_t  kSortKeyDraw2ProgramShift = kSortKeyDraw2BlendShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM;
constexpr uint64_t kSortKeyDraw2ProgramMask = uint64_t(BGFX_CONFIG_MAX_PROGRAMS - 1) << kSortKeyDraw2ProgramShift;

//
constexpr uint8_t  kSortKeyComputeSeqShift = kSortKeyDrawBitShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ;
constexpr uint64_t kSortKeyComputeSeqMask = ((uint64_t(1) << BGFX_CONFIG_SORT_KEY_NUM_BITS_SEQ) - 1) << kSortKeyComputeSeqShift;

constexpr uint8_t  kSortKeyComputeProgramShift = kSortKeyComputeSeqShift - BGFX_CONFIG_SORT_KEY_NUM_BITS_PROGRAM;
constexpr uint64_t kSortKeyComputeProgramMask = uint64_t(BGFX_CONFIG_MAX_PROGRAMS - 1) << kSortKeyComputeProgramShift;

static_assert(BGFX_CONFIG_MAX_VIEWS <= (1 << kSortKeyViewNumBits));
static_assert((BGFX_CONFIG_MAX_PROGRAMS& (BGFX_CONFIG_MAX_PROGRAMS - 1)) == 0); // Must be power of 2.
static_assert((0 // Render key mask shouldn't overlap.
	| kSortKeyViewMask
	| kSortKeyDrawBit
	| kSortKeyDrawTypeMask
	| kSortKeyDraw0BlendMask
	| kSortKeyDraw0ProgramMask
	| kSortKeyDraw0DepthMask
	) == (0
		^ kSortKeyViewMask
		^ kSortKeyDrawBit
		^ kSortKeyDrawTypeMask
		^ kSortKeyDraw0BlendMask
		^ kSortKeyDraw0ProgramMask
		^ kSortKeyDraw0DepthMask
		));
static_assert((0 // Render key mask shouldn't overlap.
	| kSortKeyViewMask
	| kSortKeyDrawBit
	| kSortKeyDrawTypeMask
	| kSortKeyDraw1DepthMask
	| kSortKeyDraw1BlendMask
	| kSortKeyDraw1ProgramMask
	) == (0
		^ kSortKeyViewMask
		^ kSortKeyDrawBit
		^ kSortKeyDrawTypeMask
		^ kSortKeyDraw1DepthMask
		^ kSortKeyDraw1BlendMask
		^ kSortKeyDraw1ProgramMask
		));
static_assert((0 // Render key mask shouldn't overlap.
	| kSortKeyViewMask
	| kSortKeyDrawBit
	| kSortKeyDrawTypeMask
	| kSortKeyDraw2SeqMask
	| kSortKeyDraw2BlendMask
	| kSortKeyDraw2ProgramMask
	) == (0
		^ kSortKeyViewMask
		^ kSortKeyDrawBit
		^ kSortKeyDrawTypeMask
		^ kSortKeyDraw2SeqMask
		^ kSortKeyDraw2BlendMask
		^ kSortKeyDraw2ProgramMask
		));
static_assert((0 // Compute key mask shouldn't overlap.
	| kSortKeyViewMask
	| kSortKeyDrawBit
	| kSortKeyComputeSeqShift
	| kSortKeyComputeProgramMask
	) == (0
		^ kSortKeyViewMask
		^ kSortKeyDrawBit
		^ kSortKeyComputeSeqShift
		^ kSortKeyComputeProgramMask
		));

// |               3               2               1               0|
// |fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210| Common
// |vvvvvvvvd                                                       |
// |       ^^                                                       |
// |       ||                                                       |
// |  view-+|                                                       |
// |        +-draw                                                  |
// |----------------------------------------------------------------| Draw Key 0 - Sort by program
// |        |kkttpppppppppdddddddddddddddddddddddddddddddd          |
// |        | ^ ^        ^                               ^          |
// |        | | |        |                               |          |
// |        | | +-blend  +-program                 depth-+          |
// |        | +-key type                                            |
// |----------------------------------------------------------------| Draw Key 1 - Sort by depth
// |        |kkddddddddddddddddddddddddddddddddttppppppppp          |
// |        | ^                               ^ ^        ^          |
// |        | |                               | +-blend  |          |
// |        | +-key type                depth-+  program-+          |
// |        |                                                       |
// |----------------------------------------------------------------| Draw Key 2 - Sequential
// |        |kkssssssssssssssssssssttppppppppp                      |
// |        | ^                   ^ ^        ^                      |
// |        | |                   | |        |                      |
// |        | +-key type      seq-+ +-blend  +-program              |
// |        |                                                       |
// |----------------------------------------------------------------| Compute Key
// |        |ssssssssssssssssssssppppppppp                          |
// |        |                   ^        ^                          |
// |        |                   |        |                          |
// |        |               seq-+        +-program                  |
// |        |                                                       |
// |--------+-------------------------------------------------------|
//

typedef uint16_t ViewId;

class Material;

struct SortKey
{
	enum Enum
	{
		SortProgram,
		SortDepth,
		SortSequence,
	};

	uint64_t EncodeDraw(Enum _type);

	uint64_t EncodeCompute();

	/// Returns true if item is compute command.
	bool decode(uint64_t _key, ViewId _viewRemap[BGFX_CONFIG_MAX_VIEWS])
	{
		//m_view = _viewRemap[(_key & kSortKeyViewMask) >> kSortKeyViewBitShift];
		//
		//if (_key & kSortKeyDrawBit)
		//{
		//	uint64_t type = _key & kSortKeyDrawTypeMask;
		//
		//	if (type == kSortKeyDrawTypeDepth)
		//	{
		//		m_program.idx = uint16_t((_key & kSortKeyDraw1ProgramMask) >> kSortKeyDraw1ProgramShift);
		//		return false;
		//	}
		//
		//	if (type == kSortKeyDrawTypeSequence)
		//	{
		//		m_program.idx = uint16_t((_key & kSortKeyDraw2ProgramMask) >> kSortKeyDraw2ProgramShift);
		//		return false;
		//	}
		//
		//	m_program.idx = uint16_t((_key & kSortKeyDraw0ProgramMask) >> kSortKeyDraw0ProgramShift);
		//	return false; // draw
		//}
		//
		//m_program.idx = uint16_t((_key & kSortKeyComputeProgramMask) >> kSortKeyComputeProgramShift);
		//return true; // compute
	}

	static ViewId decodeView(uint64_t _key)
	{
		return ViewId((_key & kSortKeyViewMask) >> kSortKeyViewBitShift);
	}

	static uint64_t remapView(uint64_t _key, ViewId _viewRemap[BGFX_CONFIG_MAX_VIEWS])
	{
		const ViewId   oldView = decodeView(_key);
		const uint64_t view = uint64_t(_viewRemap[oldView]) << kSortKeyViewBitShift;
		const uint64_t key = (_key & ~kSortKeyViewMask) | view;
		return key;
	}

	void reset()
	{
		m_depth = 0;
		m_seq = 0;
		m_pMat = nullptr;
		m_view = 0;
		m_blend = 0;
	}

	uint32_t      m_depth;
	uint32_t      m_seq;
	Material*     m_pMat;
	ViewId        m_view;
	uint8_t       m_blend;
};
#undef SORT_KEY_RENDER_DRAW

constexpr uint8_t  kBlitKeyViewShift = 32 - kSortKeyViewNumBits;
constexpr uint32_t kBlitKeyViewMask = uint32_t(BGFX_CONFIG_MAX_VIEWS - 1) << kBlitKeyViewShift;
constexpr uint8_t  kBlitKeyItemShift = 0;
constexpr uint32_t kBlitKeyItemMask = UINT16_MAX;
