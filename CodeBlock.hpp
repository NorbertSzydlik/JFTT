#pragma once
#include <sstream>
#include <string>
#include <memory>
#include <vector>

class CodeBlock;
using CodeBlockPtr = std::shared_ptr<CodeBlock>;
using CodeBlockPtrs = std::vector<CodeBlockPtr>;

class CodeBlock
{
public:
	CodeBlock();
	~CodeBlock();

	void addBlock(CodeBlockPtr const & block);
	std::string getRaw();
	void setSource(std::string code);
	void setSource(std::ostringstream& codeStream);
	inline bool isRawBlock() {
		return m_subBlocks.size() == 0;
	}

	class OptimizationInfo {};
	inline OptimizationInfo& getOptimizationInfo() { return m_optimizationInfo; }
private:
	CodeBlockPtrs m_subBlocks;
	std::string m_code;
	OptimizationInfo m_optimizationInfo;
};

inline void CodeBlock::addBlock(CodeBlockPtr const & block) {
	m_subBlocks.push_back(CodeBlockPtr(block));
}

inline std::string CodeBlock::getRaw() {
	if (isRawBlock()) {
		return m_code;
	}
	std::ostringstream raw;

	for (auto subBlock : m_subBlocks) {
		raw << subBlock->getRaw();
	}

	return raw.str();
}

inline void CodeBlock::setSource(std::string code) {
	m_code = code;
}
inline void CodeBlock::setSource(std::ostringstream& codeStream) {
	m_code = codeStream.str();
}
