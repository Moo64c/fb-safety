var _map;
/**
 * Set top value and position value of the sidebar due to the window size.
 */
function sidebar_top() {
  if (jQuery('.sidebar-container').length == 0) {
    return;
  }

  var header_height = jQuery('header').height() + 25;

  // Add the admin menu height if needed (we can't just measure the height of
  // the menu as it is not reconstructed yet when this function is called).
  if ($('body').hasClass('admin-menu')) {
    header_height = header_height + 29;
  }

  // Set the sidebar top property.
  jQuery('.sidebar-container').css("top", header_height + "px");
}

/**
 * Get next part of the articles from Drupal function and put them into columns.
 */
function getMoreArticles() {

  // Getting parameters from the "update" element's attributes.
  var pageNum = $('.update').attr('page');
  var entityType = $('.update').attr('entity_type') ? $('.update').attr('entity_type') : 0;
  var entityId = $('.update').attr('entity_id') ? $('.update').attr('entity_id') : 0;
  var articleType = $('.update').attr('article_type') ? $('.update').attr('article_type') : 0;
  var viewMode = $('.update').attr('view_mode') ? $('.update').attr('view_mode') : 0;
  $('.update').addClass("processing");

  // Incrementing page number to get the next part of the articles.
  pageNum = parseInt(pageNum) + 1;

  // Creating url to call with the parameters.
  var url = 'http://' + window.location.host + Drupal.settings.basePath + 'get-articles/' +
    entityType + '/' +
    entityId + '/' +
    articleType + '/' +
    viewMode + '/' +
    pageNum;

  $.ajax({
    type:"GET" ,
    url: url,
    success: function (success) {
      // If we got articles - put them into the columns.
      if (success.pageNum) {
        $('.update').removeClass("processing");
        $('.update').attr('page', success.pageNum);
        $('.more_articles').find('.right-column').parent().append(success.rightColumn);
        $('.more_articles').find('.mid-column').parent().append(success.midColumn);
        $('.more_articles').find('.left-column').parent().append(success.leftColumn);

        $('.more_articles').on("mouseenter", "a div.list-block", function(e) {
          showDescription(e.currentTarget);
        });

        $('.more_articles').on("mouseleave", "a div.list-block", function(e) {
          hideDescription(e.currentTarget);
        });

        addArticlesToMap(success.locations);
      }
      // If there are no articles - put "there are no articles" text into
      //  the "update" element.
      else {
        $('.update-text').find('span').html(success.data);
      }
    }
  });
}

Drupal.behaviors.checkVotes = {
  attach: function (context, settings) {

    // Get to the end of the page.
    $(window).scroll(function() {
      if ($(window).scrollTop() + $(window).height() == $(document).height() && $(document).find('.update').length && !$('.update').hasClass("processing")) {
        // Load articles.
        getMoreArticles();
      }
    });
  }
};

/**
 * Leaves the big column with the footer in the screen.
 */
jQuery(window).scroll(function() {
  if (jQuery(document).find('.cards').length && jQuery('.cards').find('.footer').length) {
    column = jQuery('.cards');
  }
  if (jQuery(document).find('.bigcol').length && jQuery('.bigcol').find('.footer').length) {
    column = jQuery('.bigcol');
  }
  if (typeof(column) != "undefined" && jQuery(document).width()>1023) {
    columnHeight = column.outerHeight(true);
    columnTop = jQuery('header').outerHeight(true)+25;

    column.css('max-width', column.outerWidth() + 'px');

    if (jQuery(document).find('#admin-menu').length) {
      columnTop += jQuery('#admin-menu').outerHeight(true);
    }

    if (jQuery(document).find('.search-form').length) {
      columnTop += jQuery('.search-form').outerHeight(true);
    }

    winHeight = jQuery(window).height();
    ctrlTop = columnHeight + columnTop - winHeight;

    if (jQuery(window).scrollTop() > ctrlTop && ctrlTop > 0){
      column.css({"position": "fixed"});
      column.css({"bottom": 0 + "px"});
    }
    else if (jQuery(window).scrollTop() > columnTop && ctrlTop < 0) {
      column.css({"position": "fixed"});
      column.css({"top": 0 + "px"});
    }
    else {
      column.css({"position": "relative"});
      column.css({"bottom": "auto"});
      column.css({"top": "auto"});
    }
  }
});

function moveMapInfoHider() {
  if (jQuery(document).find('.map-info .map-description').length && jQuery(document).find('.hide-info').length) {

    // Move zoom controls to the right side
    jQuery('.leaflet-control-container .leaflet-left.leaflet-top').toggleClass('leaflet-left').toggleClass('leaflet-right').css("top", '80px');

    var top = jQuery('.map-info .map-description').height()/2;

    jQuery('.hide-info').css("top", top + "px");
  }
}

jQuery(window)
  .resize(sidebar_top)
  .bind("load", sidebar_top)
  .bind("load", moveMapInfoHider);

jQuery(document).ready(
  function() {
    sidebar_top();

    moveMapInfoHider();

    jQuery('.hide-info').click(function() {
      var left = jQuery('.map-info .map-description').outerWidth();

      jQuery('.map-description').animate({width: 'toggle'}, 250);
      jQuery('.hide-info').toggleClass('show-info');

      if (jQuery('.hide-info').hasClass('show-info')){
        jQuery('.hide-info').css("right", "auto").css('left', left).animate({left: 0}, 250);
      }
      else {
        jQuery('.hide-info').animate({left: left}, 250);
      }
    });

    // Script for facebook like.
    (function(d, s, id) {
      var js, fjs = d.getElementsByTagName(s)[0];
      if (d.getElementById(id)) return;
      js = d.createElement(s); js.id = id;
      js.src = "//connect.facebook.net/he_HE/sdk.js#xfbml=1&version=v2.0&appId=183598425123625";
      fjs.parentNode.insertBefore(js, fjs);
    }(document, 'script', 'facebook-jssdk'));

    // Showing and hiding the side bar.
    jQuery('#sidebar-head-front, #sidebar-head-back').click(function() {
        function rotation(x) {
          return {'-webkit-transform': 'rotate(' + x + 'deg)',
                     '-moz-transform': 'rotate(' + x + 'deg)',
                      '-ms-transform': 'rotate(' + x + 'deg)',
                       '-o-transform': 'rotate(' + x + 'deg)',
                          'transform': 'rotate(' + x + 'deg)'}
        }

        var sidebar = jQuery('#sidebar');

        if ((sidebar.css("right") == "0px") || (sidebar.css("right") == "0")) {
          sidebar.animate({right: '-65px'});
          jQuery('#sidebar-head-front, #sidebar-head-back').css("padding-left", "5px");
          jQuery('#hide-front, #hide-back').css(rotation(180));
          jQuery('#hide-red-front, #hide-red-back').css(rotation(180));
        }
         else {
           sidebar.animate({right: '0px'});
           jQuery('#sidebar-head-front, #sidebar-head-back').css("padding-left", "10px");
           jQuery('#hide-front, #hide-back').css(rotation(0));
           jQuery('#hide-red-front, #hide-red-back').css(rotation(0));
         }
       });

    // Load articles by click on the "update" element.
    jQuery('.update').click(function(){
      if(!$('.update').hasClass("processing")) {
        getMoreArticles();
      }
    });

    jQuery('.search a').click(function(){
      jQuery('.search-input').css("display", "inline-block");
      jQuery('.search-input').focus();
    });

    // Showing the description.
    jQuery('a div.list-block').mouseenter(function(e) {
        showDescription(e.currentTarget)
      }
    );

    // Hiding the description.
    jQuery('a div.list-block').mouseleave(function(e) {
        hideDescription(e.currentTarget)
      }
    );

    // Showing text on the banner.
    jQuery('.page-banner').mouseleave(function() {
      var banner = jQuery(this);

      if (banner.find('.gradient').length > 0) {
        banner
          .find('.gradient')
          .animate({width: 'toggle'}, 250);
      }
    });

    // Hiding text on the banner.
    jQuery('.page-banner').mouseenter(function() {
      var banner = jQuery(this);
      if (banner.find('.gradient').length > 0) {
        banner
          .find('.gradient')
          .animate({width: 'toggle'}, 250);
      }
    });

    // If there is a map in the page - initialize it.
    if (jQuery('#map').length !== 0) {
      initialize();
    }
  }
);

function showDescription(element) {
  var li = jQuery(element);
  var name_h = 0;

  if (li.find('.name').length > 0){
    name_h = li.find('.name').outerHeight(true);
  }
  else if (li.find('.caption').length > 0) {
    name_h = li.find('.caption').outerHeight(true);
  }
  else if (li.find('.caption_white').length > 0) {
    name_h = li.find('.caption_white').outerHeight(true);
  }

  var mark_pos = (li.find('.page-mark').length > 0) ? (parseInt(li.find('.page-mark').css('top').replace('px', '')) + li.find('.page-mark').outerHeight() + 5) : 0;
  var mark_big_pos = (li.find('.page-mark-big').length > 0) ? (parseInt(li.find('.page-mark-big').css('top').replace('px', '')) + li.find('.page-mark-big').outerHeight() + 5) : 0;

  var height = li.innerHeight() - mark_pos - mark_big_pos - name_h;
  if (li.find('.description').length > 0) {
    li.find('.description').slideDown({duration: 250,
      start: function() {
        jQuery(this).css("display", "block");
        (jQuery(this).outerHeight() > height) ? jQuery(this).css("max-height", height + 'px') : jQuery(this).css("max-height",jQuery(this).outerHeight() + 'px');
      }
    } );
  }
  li.find('.page-mark').fadeIn(250);
  li.find('.triangle').fadeIn(250);
  li.find('.page-mark-big').fadeIn(250);
  li.find('.triangle-big').fadeIn(250);
}

function hideDescription(element) {
  var li = jQuery(element);
  if (li.find('.description').length > 0) {
    li.find('.description').slideUp(250);
    li.find('.page-mark').fadeOut(250);
    li.find('.triangle').fadeOut(250);
    li.find('.page-mark-big').fadeOut(250);
    li.find('.triangle-big').fadeOut(250);
  }
}

function clickOnFeature(e) {
  var layer = e.target;
  var map = _map;
  var bounds = layer.feature.properties.bounds;
  upLat = parseFloat(bounds._northEast.lat);
  rightLon = parseFloat(bounds._northEast.lng);
  downLat = parseFloat(bounds._southWest.lat);
  leftLon = parseFloat(bounds._southWest.lng);
  map.fitBounds([[upLat, rightLon], [downLat, leftLon]]);
  jQuery('div.views-field.views-field-body div.field-content h3').html(layer.feature.properties.name);
  jQuery('div.views-field.views-field-body div.field-content p').html(layer.feature.properties.description);
  moveMapInfoHider();
}

function onEachFeature(feature, layer) {
  layer.on({
    click: clickOnFeature
  });
}

// Initializing of the leaflet map.
function initialize() {
  if (jQuery('#map').length !== 0) {

    if (jQuery('#map').hasClass('map-page')) {

      // This is the map page.
      var map = new L.map('map').setView(new L.LatLng(10, 115), 5);
      _map = map;
      var geojson;



      // Add regions polygons.
      var geojsonFeature = {"type":"FeatureCollection"};
      geojsonFeature.features = [];
      Drupal.settings.regions.forEach(function(value, index, ar) {

        // Array of polygon points.
        var LatLon = [];
        if (value.geom.length == 1) {
          value.geom[0].forEach(function(point, index) {
            LatLon.push([point[0], point[1]]);
          });
        }
        else {
          value.geom.forEach(function(points, index) {
            var block = [];
            points.forEach(function(point, index) {
              block.push([point[0], point[1]])
            });
            LatLon.push(block);
          });
        }

        geojsonFeature.features.push({
          "type": "Feature",
          "properties": {
            "name": value.name,
            "description": value.description,
            "color": "#" + index * 10 + index * 10 + index * 10,
            "bounds": value.bounds
          },
          "geometry": {
            "type": "Polygon",
            "coordinates": [LatLon]
          }
        });
      });

      // Add all polygons to the map.
      geojson = L.geoJson(geojsonFeature, {onEachFeature: onEachFeature}).addTo(map);

      var assetLayerGroup = new L.LayerGroup().addTo(map);
      var conditions = Drupal.settings.conditions;
      var bounds = {
        _northEast: {
          lat: 0,
          lng: 0
        },
        _southWest: {
          lat: 0,
          lng: 0
        }
      };

      getArticlePoints(bounds, conditions, map, assetLayerGroup);

      map.on('moveend', function(e) {
        var bounds = map.getBounds();
        getArticlePoints(bounds, conditions, map, assetLayerGroup);
      });
    }
    else if (jQuery('#map').hasClass('article-map-tea')) {
      // This is the article page.
      if (Drupal.settings.location != undefined) {
        var map = new L.map('map').setView(new L.LatLng(Drupal.settings.location.lat, Drupal.settings.location.lon), 5);
        // add an OpenStreetMap tile layer
        L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
          attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
        }).addTo(map);
        if (Drupal.settings.location.lat != undefined && Drupal.settings.location.lon != undefined) {
          L.marker([Drupal.settings.location.lat, Drupal.settings.location.lon]).addTo(map);
        }
      }
    }
    else {
      // set up the map
      var map = new L.Map('map');
      _map = map;
      // create the tile layer with correct attribution
      var osmUrl = 'http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png';
      var osmAttrib = 'Map data © <a href="http://openstreetmap.org">OpenStreetMap</a> contributors';
      var osm = new L.TileLayer(osmUrl, {
        minZoom: 1,
        maxZoom: 18,
        attribution: osmAttrib
      });
      map.addLayer(osm);
      if (Drupal.settings.locations != undefined) {
        var lats = [], lons = [];
        Drupal.settings.locations.forEach(function (value, index, ar) {
          var marker = L.marker([value.lat, value.lng]).addTo(map);
          marker.bindPopup("<br>" + value.balloon);

          lats[index] = parseInt(value.lat);
          lons[index] = parseInt(value.lng);
        });
      }
      var upLat = Math.min.apply(Math, lats);
      var rightLon = Math.max.apply(Math, lons);
      var downLat = Math.max.apply(Math, lats);
      var leftLon = Math.min.apply(Math, lons);
      map.fitBounds([[upLat, rightLon], [downLat, leftLon]]);
    }
  }
}

function getArticlePoints(bounds, conditions, map, assetLayerGroup) {

  assetLayerGroup.clearLayers();

  L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
  }).addTo(map);

  var url = 'http://' + window.location.host + Drupal.settings.basePath + 'get-articles-map-points/' +
    bounds._northEast.lat + '/' +
    bounds._northEast.lng + '/' +
    bounds._southWest.lat + '/' +
    bounds._southWest.lng + '/' +
    conditions.tid + '?' +
    'search_api_views_fulltext=' + conditions.searchWord;

  $.ajax({
    type: "GET",
    url: url,
    success: function (success) {
      // If we got articles - put them into the map.
      if (success.length) {

        var lats = [], lons = [];

        success.forEach(function (value, index, ar) {
          lat = value.field_geo.lat;
          lon = value.field_geo.lon;
          lats[index] = parseInt(lat);
          lons[index] = parseInt(lon);
          marker = new L.marker([lat, lon]).bindPopup("<br>" + value.popup);
          marker.on("click", function(e) {
            changeMapDescription(value);
          });

          assetLayerGroup.addLayer(marker);
        });

        if (bounds._northEast.lat == 0 && bounds._northEast.lng == 0) {
          var upLat = Math.min.apply(Math, lats);
          var rightLon = Math.max.apply(Math, lons);
          var downLat = Math.max.apply(Math, lats);
          var leftLon = Math.min.apply(Math, lons);

          map.fitBounds([[upLat, rightLon], [downLat, leftLon]]);
        }
      }
      // If there are no articles
      else {
        console.log('no result');
      }
    }
  });
}

/**
 * Add locations markers to the existing map.
 *
 * @param locations
 *  Array of markers - coordinates and content for the popup.
 */
function addArticlesToMap(locations) {
  if (jQuery('#map').length !== 0) {
    Drupal.settings.locations = Drupal.settings.locations.concat(locations);
    var map = _map;
    var lats = [], lons = [];
    locations.forEach(function (value, index, ar) {
      var marker = new L.marker([value.lat, value.lng]).bindPopup("<br>" + value.balloon);

      map.addLayer(marker);
    });
    Drupal.settings.locations.forEach(function (value, index, ar) {
      lats[index] = parseInt(value.lat);
      lons[index] = parseInt(value.lng);
    });

    var upLat = Math.min.apply(Math, lats);
    var rightLon = Math.max.apply(Math, lons);
    var downLat = Math.max.apply(Math, lats);
    var leftLon = Math.min.apply(Math, lons);
    map.fitBounds([[upLat, rightLon], [downLat, leftLon]]);
  }
}

// Handle sidebar card flip effect.
jQuery(document).ready(function(){
  jQuery('a.flip').click(function(event){
    jQuery('#sidebar').toggleClass('rotate-3d');
    event.preventDefault();
  });
});

(function ($) {

/**
 * Override Drupal.overlay.makeDocumentUntabbable
 *
 * Since we are using jQuery higher than 1.3, jQuery.browser doesn't work.
 * So instead we override the original overlay function and mock
 * jQuery.browser.msie.
 */
Drupal.overlay.makeDocumentUntabbable = function (context) {
  // Manipulating tabindexes for the entire document is unacceptably slow in IE6
  // and IE7, so in those browsers, the underlying page will still be reachable
  // via the tab key. However, we still make the links within the Disable
  // message unreachable, because the same message also exists within the
  // child document. The duplicate copy in the underlying document is only for
  // assisting screen-reader users navigating the document with reading commands
  // that follow markup order rather than tab order.
  jQuery.browser = {};
  jQuery.browser.msie = false;
  jQuery.browser.version = 0;
  if (navigator.userAgent.match(/MSIE ([0-9]+)\./)) {
    jQuery.browser.msie = true;
    jQuery.browser.version = RegExp.$1;
  }

  if (jQuery.browser.msie && parseInt(jQuery.browser.version, 10) < 8) {
    $('#overlay-disable-message a', context).attr('tabindex', -1);
    return;
  }

  context = context || document.body;
  var $overlay, $tabbable, $hasTabindex;

  // Determine which elements on the page already have a tabindex.
  $hasTabindex = $('[tabindex] :not(.overlay-element)', context);
  // Record the tabindex for each element, so we can restore it later.
  $hasTabindex.each(Drupal.overlay._recordTabindex);
  // Add the tabbable elements from the current context to any that we might
  // have previously recorded.
  Drupal.overlay._hasTabindex = $hasTabindex.add(Drupal.overlay._hasTabindex);

  // Set tabindex to -1 on everything outside the overlay and toolbars, so that
  // the underlying page is unreachable.

  // By default, browsers make a, area, button, input, object, select, textarea,
  // and iframe elements reachable via the tab key.
  $tabbable = $('a, area, button, input, object, select, textarea, iframe');
  // If another element (like a div) has a tabindex, it's also tabbable.
  $tabbable = $tabbable.add($hasTabindex);
  // Leave links inside the overlay and toolbars alone.
  $overlay = $('.overlay-element, #overlay-container, .overlay-displace-top, .overlay-displace-bottom').find('*');
  $tabbable = $tabbable.not($overlay);
  // We now have a list of everything in the underlying document that could
  // possibly be reachable via the tab key. Make it all unreachable.
  $tabbable.attr('tabindex', -1);
};

})(jQuery);

/**
 * Load the given URL into the overlay iframe.
 *
 * Use this method to change the URL being loaded in the overlay if it is
 * already open.
 *
 * @return
 *   TRUE if URL is loaded into the overlay, FALSE otherwise.
 */
Drupal.overlay.load = function (url) {
  if (!this.isOpen) {
    return false;
  }

  // Set parent popup for this popup if exists.
  var checkHash = window.location.hash.match(/popup\/node\/([0-9]+)/);
  if (checkHash && checkHash[1] !== "undefined") {
    this.lastGalleryPopup = window.location.hash;
  }

  // Check that this popup is image slideshow.
  var checkHash = window.location.hash.match(/popup\/node\/image\/([0-9]+)%3Farticle_id%3D([0-9]+)/);
  if (checkHash && checkHash[2] !== "undefined") {
    this.isImageShow = true;
  } 
  else {
    this.isImageShow = false;
  }

  // Allow other scripts to respond to this event.
  $(document).trigger('drupalOverlayBeforeLoad');

  $(document.documentElement).addClass('overlay-loading');

  // The contentDocument property is not supported in IE until IE8.
  var iframeDocument = this.inactiveFrame[0].contentDocument || this.inactiveFrame[0].contentWindow.document;

  // location.replace doesn't create a history entry. location.href does.
  // In this case, we want location.replace, as we're creating the history
  // entry using URL fragments.
  iframeDocument.location.replace(url);

  return true;
};

/**
 * Close the overlay and remove markup related to it from the document.
 *
 * @return
 *   TRUE if the overlay was closed, FALSE otherwise.
 */
Drupal.overlay.close = function () {
  // Prevent double execution when close is requested more than once.
  if (!this.isOpen || this.isClosing) {
    return false;
  }

  // If this popup is ImageShow and preview popup is exists
  // Then close this popup and show preview.
  if (this.isImageShow && this.lastGalleryPopup) {
    window.location.hash = this.lastGalleryPopup;
    return false;
  }

  // Allow other scripts to respond to this event.
  var event = $.Event('drupalOverlayBeforeClose');
  $(document).trigger(event);
  // If a handler returned false, the close will be prevented.
  if (event.isDefaultPrevented()) {
    return false;
  }

  this.isClosing = true;
  this.isOpen = false;
  $(document.documentElement).removeClass('overlay-open');
  // Restore the original document title.
  document.title = this.originalTitle;
  this.makeDocumentTabbable();

  // Allow other scripts to respond to this event.
  $(document).trigger('drupalOverlayClose');

  // When the iframe is still loading don't destroy it immediately but after
  // the content is loaded (see Drupal.overlay.loadChild).
  if (!this.isLoading) {
    this.destroy();
    this.isClosing = false;
  }
  return true;
};


function changeMapDescription(value) {
  var text = "";
  text += "<h3>" + value.title + "</h3>";
  text += value.summary;
  text += value.readMore;

  jQuery(".map-description .view-content .views-row-1 .views-field-body .field-content").html(text);
}
