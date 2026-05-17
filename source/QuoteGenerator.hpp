
#pragma once

#include <juce_core/juce_core.h>
#include <optional>
#include <random>

struct GrimlockQuote
{
    juce::String quote, episodeCode, episodeName, link, imageCode;

    void printDebugInfo()
    {
        DBG(episodeCode);
        DBG(episodeName);
        DBG(quote);
        DBG(link);
        DBG(imageCode);
    }
};

class QuoteGenerator
{
    public:

        [[nodiscard]] std::optional<GrimlockQuote> getQuoteFromIndex(const juce::var& var, int index)
        {
            jassert(var.isArray());

            auto array = var.getArray();

            quoteIndex = index;

            for(auto& e : *array)
            {
                auto* quoteArray = e.getProperty("quotes",{}).getArray();
                jassert(quoteArray != nullptr);
                if(quoteArray == nullptr) {
                    continue;
                }

                auto size = quoteArray->size();
                if(index >= size)
                {
                    index -= size;
                }
                else
                {
                    quoteInfo.episodeCode = e.getProperty("episode", "episode not found").toString();
                    quoteInfo.episodeName = e.getProperty("title", "title not found").toString();
                    quoteInfo.link = e.getProperty("link", "link not found").toString();

                    auto qArray = *(quoteArray);
                    quoteInfo.quote = qArray[index].getProperty(juce::String(index), juce::var()).toString();

                    if(quoteInfo.quote.isEmpty())
                        return std::nullopt;

                    quoteInfo.imageCode = quoteInfo.episodeCode + "_";
                    if(quoteInfo.imageCode == "Movie_") {
                        quoteInfo.imageCode = "movie_";
                    }
                    index += 1;
                    if(index < 10) {
                        quoteInfo.imageCode += "0";
                    }
                    quoteInfo.imageCode += juce::String(index) + "_png";

                    return quoteInfo;
                }
            }

            return std::nullopt;
        }

        [[nodiscard]] std::optional<GrimlockQuote> generateRandomQuote(const juce::var& var)
        {
            jassert(var.isArray());

            auto array = var.getArray();
            totalQuotes = 0;
            for(auto& e : *array) {
                if(e.hasProperty("quotes")) {
                    totalQuotes += e.getProperty("quotes",{}).getArray()->size();
                }
            }

            std::random_device rd;
            std::uniform_int_distribution<int> dist (0, totalQuotes - 1);
            std::mt19937 gen(rd());

            quoteIndex = dist(gen);

            return getQuoteFromIndex(var, quoteIndex);
        }

        int getQuoteIndex() const { return quoteIndex; }

    private:

        int totalQuotes = 0;
        int quoteIndex = 0;
        GrimlockQuote quoteInfo;
};
