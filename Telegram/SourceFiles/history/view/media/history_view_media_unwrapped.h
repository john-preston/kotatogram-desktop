/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "history/view/media/history_view_media.h"
#include "base/weak_ptr.h"
#include "base/timer.h"

struct HistoryMessageVia;
struct HistoryMessageReply;
struct HistoryMessageForwarded;

namespace HistoryView {

class UnwrappedMedia final : public Media {
public:
	class Content {
	public:
		[[nodiscard]] virtual QSize size() = 0;

		virtual void draw(Painter &p, const QRect &r, bool selected) = 0;

		[[nodiscard]] virtual ClickHandlerPtr link() {
			return nullptr;
		}

		[[nodiscard]] virtual DocumentData *document() {
			return nullptr;
		}
		virtual void clearStickerLoopPlayed() {
		}
		virtual void unloadHeavyPart() {
		}
		virtual void refreshLink() {
		}
		[[nodiscard]] virtual bool hidesForwardedInfo() {
			return false;
		}
		[[nodiscard]] virtual bool alwaysShowOutTimestamp() {
			return false;
		}
		virtual ~Content() = 0;
	};

	UnwrappedMedia(
		not_null<Element*> parent,
		std::unique_ptr<Content> content);

	void draw(Painter &p, const QRect &r, TextSelection selection, crl::time ms) const override;
	PointState pointState(QPoint point) const override;
	TextState textState(QPoint point, StateRequest request) const override;

	bool toggleSelectionByHandlerClick(const ClickHandlerPtr &p) const override {
		return true;
	}
	bool dragItem() const override {
		return true;
	}
	bool dragItemByHandler(const ClickHandlerPtr &p) const override {
		return true;
	}

	DocumentData *getDocument() const override {
		return _content->document();
	}

	bool needsBubble() const override {
		return false;
	}
	bool customInfoLayout() const override {
		return true;
	}
	bool hidesForwardedInfo() const override {
		return _content->hidesForwardedInfo();
	}
	void clearStickerLoopPlayed() override {
		_content->clearStickerLoopPlayed();
	}

	void unloadHeavyPart() override {
		_content->unloadHeavyPart();
	}

private:
	struct SurroundingInfo {
		int height = 0;
		int forwardedHeight = 0;
		bool forwardedBreakEverywhere = false;

		explicit operator bool() const {
			return (height > 0);
		}
	};
	[[nodiscard]] SurroundingInfo surroundingInfo(
		const HistoryMessageVia *via,
		const HistoryMessageReply *reply,
		const HistoryMessageForwarded *forwarded,
		int outerw) const;
	void drawSurrounding(
		Painter &p,
		const QRect &inner,
		bool selected,
		const HistoryMessageVia *via,
		const HistoryMessageReply *reply,
		const HistoryMessageForwarded *forwarded) const;

	QSize countOptimalSize() override;
	QSize countCurrentSize(int newWidth) override;

	bool needInfoDisplay() const;
	int additionalWidth(
		const HistoryMessageVia *via,
		const HistoryMessageReply *reply,
		const HistoryMessageForwarded *forwarded) const;

	inline int calculateFullRight(const QRect &inner) const;
	inline QPoint calculateFastActionPosition(
		int fullBottom,
		int replyRight,
		int fullRight) const;

	const HistoryMessageForwarded *getDisplayedForwardedInfo() const;

	std::unique_ptr<Content> _content;
	QSize _contentSize;

	rpl::lifetime _lifetime;

};

} // namespace HistoryView
