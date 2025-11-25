#include "sortkey.h"
#include "material.h"

uint64_t SortKey::EncodeDraw(Enum _type)
{
	switch (_type)
	{
	case SortProgram:
	{
		const uint64_t depth = (uint64_t(m_depth) << kSortKeyDraw0DepthShift) & kSortKeyDraw0DepthMask;
		const uint64_t program = (uint64_t(m_pMat->GetHandle()) << kSortKeyDraw0ProgramShift) & kSortKeyDraw0ProgramMask;
		const uint64_t blend = (uint64_t(m_blend) << kSortKeyDraw0BlendShift) & kSortKeyDraw0BlendMask;
		const uint64_t view = (uint64_t(m_view) << kSortKeyViewBitShift) & kSortKeyViewMask;
		const uint64_t key = view | kSortKeyDrawBit | kSortKeyDrawTypeProgram | blend | program | depth;

		return key;
	}
	break;

	case SortDepth:
	{
		const uint64_t depth = (uint64_t(m_depth) << kSortKeyDraw1DepthShift) & kSortKeyDraw1DepthMask;
		const uint64_t program = (uint64_t(m_pMat->GetHandle()) << kSortKeyDraw1ProgramShift) & kSortKeyDraw1ProgramMask;
		const uint64_t blend = (uint64_t(m_blend) << kSortKeyDraw1BlendShift) & kSortKeyDraw1BlendMask;
		const uint64_t view = (uint64_t(m_view) << kSortKeyViewBitShift) & kSortKeyViewMask;
		const uint64_t key = view | kSortKeyDrawBit | kSortKeyDrawTypeDepth | depth | blend | program;
		return key;
	}
	break;

	case SortSequence:
	{
		const uint64_t seq = (uint64_t(m_seq) << kSortKeyDraw2SeqShift) & kSortKeyDraw2SeqMask;
		const uint64_t program = (uint64_t(m_pMat->GetHandle()) << kSortKeyDraw2ProgramShift) & kSortKeyDraw2ProgramMask;
		const uint64_t blend = (uint64_t(m_blend) << kSortKeyDraw2BlendShift) & kSortKeyDraw2BlendMask;
		const uint64_t view = (uint64_t(m_view) << kSortKeyViewBitShift) & kSortKeyViewMask;
		const uint64_t key = view | kSortKeyDrawBit | kSortKeyDrawTypeSequence | seq | blend | program;

		//BX_ASSERT(seq == (uint64_t(m_seq) << kSortKeyDraw2SeqShift)
		//	, "SortKey error, sequence is truncated (m_seq: %d)."
		//	, m_seq
		//);

		return key;
	}
	break;
	}

	//static_assert(false, "You should not be here.");
	return 0;
}

uint64_t SortKey::EncodeCompute()
{
	const uint64_t program = (uint64_t(m_pMat->GetHandle()) << kSortKeyComputeProgramShift) & kSortKeyComputeProgramMask;
	const uint64_t seq = (uint64_t(m_seq) << kSortKeyComputeSeqShift) & kSortKeyComputeSeqMask;
	const uint64_t view = (uint64_t(m_view) << kSortKeyViewBitShift) & kSortKeyViewMask;
	const uint64_t key = program | seq | view;

	//BX_ASSERT(seq == (uint64_t(m_seq) << kSortKeyComputeSeqShift)
	//	, "SortKey error, sequence is truncated (m_seq: %d)."
	//	, m_seq
	//);

	return key;
}