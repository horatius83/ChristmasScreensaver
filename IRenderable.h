#ifndef IRENDERABLE_H
#define IRENDERABLE_H

namespace Rendering {
	class IRenderable {
	public:
		virtual void BeginRendering() = 0;
		virtual void Render() = 0;
		virtual void EndRendering() = 0;
	};
}

#endif