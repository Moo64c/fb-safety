<div class="form-results">
  <div class="heading">
    <div class="holder">
      <?php print $switcher; ?>
    </div>
  </div>

  <div class="box box-content">
    <h1 class="box-label"><?php print $page_element['title']; ?></h1>
    <div class="box-inside">
      <div class="description-wider">
        <div class="description">
          <?php print $page_element['body']; ?>
        </div>
      </div>
    </div>
  </div>

  <div id="boxes-wrapper">
    <?php print $results; ?>

    <!-- We need this link for the infinite scroll plugin -->
    <a id="next-page" href="#"></a>
    <div class="clearfix"></div>
  </div>

</div>
