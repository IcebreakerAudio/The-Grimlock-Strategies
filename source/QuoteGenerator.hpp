
#pragma once

#include <juce_core/juce_core.h>
#include <random>

struct GrimlockQuote
{
    bool isValid = false;
    juce::String quote, episodeCode, episodeName, link, imageCode;

    void printDebugInfo()
    {
        if(!isValid)
        {
            DBG("No Quote");
            return;
        }

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

        GrimlockQuote getQuoteFromIndex(juce::var& var, int index)
        {
            jassert(var.isArray());

            auto array = var.getArray();

            quoteIndex = index;

            for(auto& e : *array)
            {
                auto size = e.getProperty("quotes",{}).getArray()->size();
                if(index >= size)
                {
                    index -= size;
                }
                else
                {
                    quoteInfo.episodeCode = e.getProperty("episode", "episode not found").toString();
                    quoteInfo.episodeName = e.getProperty("title", "title not found").toString();
                    quoteInfo.link = e.getProperty("link", "link not found").toString();

                    auto qArray = *(e.getProperty("quotes",{}).getArray());
                    quoteInfo.quote = qArray[index].getProperty(juce::String(index), "quote not found").toString();

                    quoteInfo.imageCode = quoteInfo.episodeCode + "_";
                    if(quoteInfo.imageCode.compare("Movie_") == 0) {
                        quoteInfo.imageCode = "movie_";
                    }
                    index += 1;
                    if(index < 10) {
                        quoteInfo.imageCode += "0";
                    }
                    quoteInfo.imageCode += juce::String(index) + "_png";

                    quoteInfo.isValid = quoteInfo.quote.compare("quote not found") != 0;

                    return quoteInfo;
                }
            }

            quoteInfo.isValid = false;

            return quoteInfo;
        }

        GrimlockQuote generateRandomQuote(juce::var& var)
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

        int getQuoteIndex() { return quoteIndex; }

    private:

        int totalQuotes = 0;
        int quoteIndex = 0;
        GrimlockQuote quoteInfo;
};
